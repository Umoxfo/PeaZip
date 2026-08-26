module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

module PeaZip.ShellExt:GenericCommand;

import std;
import winrt_base;

import PeaZip.ShellExt.DllEnv;

import :Localizer;
import :types;

namespace PeaZip::ShellExt
{
namespace fs = std::filesystem;

GenericExplorerCommand::GenericExplorerCommand(CommandMetadata metadata) noexcept : m_metadata(std::move(metadata))
{
}

std::vector<std::wstring> GenericExplorerCommand::ExtractFilePaths(
    winrt::com_ptr<IShellItemArray> const& shellItemArray) const noexcept
{
    if (shellItemArray == nullptr)
    {
        return {};
    }

    DWORD count = 0;
    if (FAILED(shellItemArray->GetCount(&count)) || count == 0)
    {
        return {};
    }

    std::vector<std::wstring> filePaths;
    filePaths.reserve(count);

    for (auto i : std::ranges::views::iota(DWORD{0}, count))
    {
        winrt::com_ptr<IShellItem> item;
        if (SUCCEEDED(shellItemArray->GetItemAt(i, item.put())))
        {
            wil::unique_cotaskmem_string rawPath;
            if (SUCCEEDED(item->GetDisplayName(SIGDN_FILESYSPATH, rawPath.put())) && rawPath)
            {
                filePaths.emplace_back(rawPath.get());
            }
        }
    }

    return filePaths;
}

/// <summary>
/// Gets the localized title of the command for display in the Explorer context menu.
/// Caches the title string upon the first invocation to avoid redundant resource lookups.
/// </summary>
/// <param name="ppszName">Receives a pointer to the allocated string buffer containing the title.</param>
/// <returns><c>HRESULT</c> indicating success or failure.</returns>
IFACEMETHODIMP GenericExplorerCommand::GetTitle(IShellItemArray*, LPWSTR* ppszName) noexcept
{
    if (ppszName == nullptr)
    {
        return E_POINTER;
    }

    // Load and cache the title string if it has not been loaded yet
    if (m_cachedTitle.empty())
    {
        // Use Localizer to retrieve the resource string
        m_cachedTitle = GetLocalizedString(m_metadata.labelKey, m_metadata.fallbackLabel);
    }

    // Allocate and copy string using CoTaskMemAlloc to comply with COM ABI rules
    auto title = wil::make_cotaskmem_string_nothrow(m_cachedTitle.c_str());
    if (title == nullptr) [[unlikely]]
    {
        *ppszName = nullptr;
        return E_OUTOFMEMORY;
    }

    // Transfer ownership to the caller (Explorer) by releasing the raw pointer
    *ppszName = title.release();
    return S_OK;
}

IFACEMETHODIMP GenericExplorerCommand::GetIcon(IShellItemArray*, LPWSTR* ppszIcon) noexcept
{
    if (ppszIcon == nullptr)
    {
        return E_POINTER;
    }
    *ppszIcon = nullptr;

    try
    {
        auto const& basePathResult = DllEnv::GetAppBasePath();
        if (!basePathResult.has_value()) [[unlikely]]
        {
            return basePathResult.error();
        }

        const auto iconFullPath = basePathResult.value() / m_metadata.iconPath;

        // 3. Check file existence using the std::error_code overload (noexcept)
        if (std::error_code ec; !fs::exists(iconFullPath, ec)) [[unlikely]]
        {
            if (!ec)
            {
                // The path simply does not exist (No OS error occurred)
                return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
            }

            // Ensure the error code originates from the OS (Win32) before conversion
            if (ec.category() == std::system_category())
            {
                return HRESULT_FROM_WIN32(ec.value());
            }

            // Fallback for generic_category or other error domains
            return E_FAIL;
        }

        // エクスプローラー用のカンマ区切り文字列を生成
        auto const finalIconStr = std::format(L"{},{}", iconFullPath.wstring(), m_metadata.iconIndex);

        // Safely allocate the CoTaskMem memory block using WIL's RAII wrapper
        auto allocatedStr = wil::make_cotaskmem_string_nothrow(finalIconStr.c_str());
        RETURN_IF_NULL_ALLOC(allocatedStr);

        // Transfer ownership to the caller (Explorer) by releasing the raw pointer
        *ppszIcon = allocatedStr.release();
        return S_OK;
    }
    catch (...)
    {
        // Convert any exception to an HRESULT.
        return winrt::to_hresult();
    }
}

IFACEMETHODIMP GenericExplorerCommand::GetState(IShellItemArray*, BOOL, EXPCMDSTATE* pCmdState) noexcept
{
    if (pCmdState == nullptr)
        return E_POINTER;

    *pCmdState = ECS_ENABLED;
    return S_OK;
}

IFACEMETHODIMP GenericExplorerCommand::GetFlags(EXPCMDFLAGS* pFlags) noexcept
{
    if (pFlags == nullptr)
        return E_POINTER;

    *pFlags = m_metadata.flags;
    return S_OK;
}

IFACEMETHODIMP GenericExplorerCommand::Invoke(IShellItemArray* psiItemArray, IBindCtx*) noexcept
{
    if (psiItemArray == nullptr)
        return S_OK;

    try
    {
        winrt::com_ptr<IShellItemArray> shellItemArray;
        shellItemArray.copy_from(psiItemArray);

        // STAスレッド上で安全にパスを抽出
        auto filePaths = ExtractFilePaths(shellItemArray);
        if (filePaths.empty())
        {
            return S_OK;
        }

        // 非同期処理が完了するまで、このコマンドインスタンス自身の寿命を延ばす (参照カウントの増加)
        auto strongThis = get_strong();

        // fire_and_forget を使用して非同期コルーチン(ラムダ)を起動する
        [strongThis](std::vector<std::wstring> paths) -> winrt::fire_and_forget {
            try
            {
                // バックグラウンドスレッド（MTA）へ切り替え
                co_await winrt::resume_background();

                // キャッシュされたDLL親フォルダのパスを取得
                auto const& basePathResult = DllEnv::GetAppBasePath();
                if (!basePathResult.has_value()) [[unlikely]]
                {
                    co_return;
                }
                auto const& basePath = basePathResult.value();

                // 実行ファイルのフルパス構築
                const fs::path fullExePath = basePath / strongThis->m_metadata.command;

#pragma region Argument Construction
                // 1. const wchar_t* を wstring_view で受け取り、一時的な文字列コピーを完全に排除
                const std::wstring_view argsView{strongThis->m_metadata.arguments};

                // 2. std::ranges パイプラインを用いて、必要な正確なバッファサイズを事前計算する
                // L" " (1文字) + L"\"" (1文字) + パスの長さ + L"\"" (1文字) = path.size() + 3
                const size_t requiredSize = std::ranges::fold_left(
                    paths, argsView.size(), [](size_t acc, auto const& path) { return acc + path.size() + 3uz; });

                // 計算された必要なサイズが、基本引数のサイズより確実に大きいことをコンパイラに教える
                [[assume(requiredSize > argsView.size())]];

                // 3. 正確なサイズでバッファを1回だけ確保し、再アロケーションを完全に防ぐ
                std::wstring sb;
                sb.reserve(requiredSize);

                // 4. 引数文字列を構築
                sb.append_range(argsView);
                std::ranges::for_each(paths, [&sb](auto const& path) {
                    [[assume(!path.empty())]];
                    sb.append(LR"( ")").append(path).append(1, L'"');
                });
                [[assume(sb.size() == requiredSize)]];
#pragma endregion

                // アプリ起動
                SHELLEXECUTEINFOW sei{
                    .cbSize = sizeof(sei),
                    .fMask = SEE_MASK_DEFAULT,
                    .hwnd = nullptr,
                    .lpVerb = L"open",
                    .lpFile = fullExePath.c_str(),
                    .lpParameters = sb.c_str(),
                    .nShow = SW_SHOWNORMAL
                };

                ::ShellExecuteExW(&sei);
            }
            catch (...)
            {
            }
        }(std::move(filePaths));

        return S_OK;
    }
    catch (...)
    {
        return winrt::to_hresult();
    }
}

#pragma region Standard Interface Stubs
IFACEMETHODIMP GenericExplorerCommand::GetToolTip(IShellItemArray*, LPWSTR* ppszInfotip) noexcept
{
    if (ppszInfotip == nullptr)
        return E_INVALIDARG;

    *ppszInfotip = nullptr;
    return E_NOTIMPL;
}

IFACEMETHODIMP GenericExplorerCommand::GetCanonicalName(GUID* pguidCommandName) noexcept
{
    if (pguidCommandName == nullptr)
        return E_INVALIDARG;

    *pguidCommandName = GUID_NULL;
    return S_OK;
}

IFACEMETHODIMP GenericExplorerCommand::EnumSubCommands(IEnumExplorerCommand** ppEnum) noexcept
{
    *ppEnum = nullptr;
    return E_NOTIMPL;
}
#pragma endregion
} // namespace PeaZip::ShellExt
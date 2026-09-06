module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

module PeaZip.ShellExt:SubCommand;

import std;
import winrt_base;

import :Localizer;
import :types;
import :utils;

using namespace PeaZip::ShellExt::Localizer;
using namespace PeaZip::ShellExt::utils;

namespace PeaZip::ShellExt::SubCommand
{
namespace fs = std::filesystem;

std::vector<std::wstring> GenericSubCommand::ExtractFilePaths(IShellItemArray* shellItemArray) const noexcept
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
                const wchar_t* pPath = rawPath.get();

                if (pPath[0] != L'\0')
                {
                    filePaths.emplace_back(pPath);
                }
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
IFACEMETHODIMP GenericSubCommand::GetTitle(IShellItemArray*, LPWSTR* ppszName) noexcept
{
    *ppszName = nullptr;

    try
    {
        // Load and cache the title string if it has not been loaded yet
        if (m_cachedTitle.empty())
        {
            // Use Localizer to retrieve the resource string
            m_cachedTitle = GetLocalizedString(m_metadata.labelKey, m_metadata.fallbackLabel);
        }

        // Allocate and copy string using CoTaskMemAlloc to comply with COM ABI rules
        auto title = wil::make_cotaskmem_string_nothrow(m_cachedTitle.c_str(), m_cachedTitle.size());
        if (!title) [[unlikely]]
        {
            return E_OUTOFMEMORY;
        }

        // Transfer ownership to the caller (Explorer) by releasing the raw pointer
        *ppszName = title.release();
        return S_OK;
    }
    catch (...)
    {
        // Convert any exception to an HRESULT.
        return winrt::to_hresult();
    }
}

IFACEMETHODIMP GenericSubCommand::GetIcon(IShellItemArray*, LPWSTR* ppszIcon) noexcept
{
    *ppszIcon = nullptr;

    try
    {
        auto const& basePathResult = get_module_directory_path();
        if (!basePathResult) [[unlikely]]
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

        wil::unique_cotaskmem_string target;
        RETURN_IF_FAILED(wil::str_printf_nothrow(target, L"%ws,%d", iconFullPath.wstring(), m_metadata.iconIndex));

        // Transfer ownership to the caller (Explorer) by releasing the raw pointer
        *ppszIcon = target.release();
        return S_OK;
    }
    catch (...)
    {
        // Convert any exception to an HRESULT.
        return winrt::to_hresult();
    }
}

IFACEMETHODIMP GenericSubCommand::Invoke(IShellItemArray* psiItemArray, IBindCtx*) noexcept
{
    if (psiItemArray == nullptr)
    {
        return S_OK;
    }

    try
    {
        // STAスレッド上で安全にパスを抽出
        auto filePaths = ExtractFilePaths(psiItemArray);
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
                auto const& basePathResult = get_module_directory_path();
                if (!basePathResult) [[unlikely]]
                {
                    co_return;
                }
                auto const& basePath = basePathResult.value();

                // 実行ファイルのフルパス構築
                const fs::path exePath = basePath / strongThis->m_metadata.command;

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
                    .lpFile = exePath.c_str(),
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
} // namespace PeaZip::ShellExt::SubCommand
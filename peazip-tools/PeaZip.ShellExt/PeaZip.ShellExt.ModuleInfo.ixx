module;
#include "pch.h"

export module PeaZip.ShellExt.ModuleInfo;

import std;

namespace PeaZip::ShellExt::ModuleInfo
{
    // DLL自身の HMODULE を安全に取得するヘルパー
    [[nodiscard]] inline HMODULE GetCurrentModuleHandle() noexcept
    {
        HMODULE hModule = nullptr;

        // この関数自身のアドレスを利用して、所属するDLLのハンドルを取得する
        ::GetModuleHandleExW(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(&GetCurrentModuleHandle),
            &hModule);
        return hModule;
    }

    export inline const std::expected<std::filesystem::path, HRESULT>& GetAppBasePath() noexcept
    {
        static const auto cachedResult = []() -> std::expected<std::filesystem::path, HRESULT>
            {
                HMODULE hMod = GetCurrentModuleHandle();
                if (!hMod) [[unlikely]]
                {
                    return std::unexpected{ HRESULT_FROM_WIN32(::GetLastError()) };
                }

                // 例外をスローしない、unique_cotaskmem_string を受けるバリアントに変更
                wil::unique_cotaskmem_string dllPath;
                HRESULT hr = wil::GetModuleFileNameW(hMod, dllPath);
                if (FAILED(hr)) [[unlikely]]
                {
                    return std::unexpected(hr);
                }

                // dllPath.get() を使用して std::filesystem::path を構築
                std::filesystem::path p = dllPath.get();
                return p.parent_path();
            }();

        return cachedResult;
    }
}

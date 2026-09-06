module;
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <wil/win32_helpers.h>

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

export module PeaZip.ShellExt:utils;

import std;
import winrt_base;

namespace PeaZip::ShellExt::utils
{
namespace fs = std::filesystem;

export inline [[nodiscard]] std::expected<fs::path, HRESULT> const& get_module_directory_path()
{
    static const auto s_basePathResult = []() -> std::expected<fs::path, HRESULT> {
        wil::unique_cotaskmem_string dllPath;

        // 1. Retrieve the module path via WIL (Early exit on failure)
        HRESULT hr = wil::GetModuleFileNameW(wil::GetModuleInstanceHandle(), dllPath);
        if (FAILED(hr)) [[unlikely]]
        {
            return std::unexpected(hr);
        }

        // 2. Safeguard operations that may throw exceptions (e.g., memory allocation, encoding conversion)
        try
        {
            // Construct the path and return its parent (Triggers internal memory allocation)
            return fs::path(dllPath.get()).parent_path();
        }
        catch (const std::bad_alloc&)
        {
            // Handle Out of Memory (OOM) situations safely
            return std::unexpected(E_OUTOFMEMORY);
        }
        catch (...)
        {
            // Catch C++/WinRT exceptions (winrt::hresult_error) and C++26 encoding conversion errors,
            // then map them to an appropriate HRESULT code automatically.
            return std::unexpected(winrt::to_hresult());
        }
    }();

    // Return the cached `expected` object by "reference" (zero-copy)
    return s_basePathResult;
}
} // namespace PeaZip::ShellExt::utils
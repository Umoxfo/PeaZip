module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/Microsoft.Windows.ApplicationModel.Resources.h>
#endif

export module PeaZip.ShellExt:Localizer;

import std;
import winrt.Microsoft.Windows.ApplicationModel.Resources;

using namespace winrt::Microsoft::Windows::ApplicationModel::Resources;

namespace PeaZip::ShellExt
{
/// <summary>
/// Retrieves a localized string resource corresponding to the specified key using MRT Core <c>ResourceLoader</c>.
/// </summary>
/// <param name="resourceKey">Resource key defined in the <c>.resw</c> file.</param>
/// <param name="fallbackText">Fallback string used if the resource lookup fails.</param>
/// <returns>Resolved localized string, or the fallback string.</returns>
export winrt::hstring GetLocalizedString(std::wstring_view resourceKey, std::wstring_view fallbackText = L"") noexcept
{
    // Thread-safe lazy-initialization pattern using C++ static local variable
    static const ResourceLoader loader;

    try
    {
        const auto result = loader.GetString(resourceKey);
        if (!result.empty())
        {
            return result;
        }
    }
    catch (...)
    {
        // Fall back to predefined label if resource lookup fails or throws
    }

    return winrt::hstring{fallbackText};
}
} // namespace PeaZip::ShellExt
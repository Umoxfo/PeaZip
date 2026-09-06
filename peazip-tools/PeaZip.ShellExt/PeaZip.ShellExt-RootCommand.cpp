module;
#include "pch.h"

#include <wil/registry.h>

#ifdef __INTELLISENSE__
#include <winrt/Windows.Foundation.h>
#include <winrt/Microsoft.Windows.Storage.h>
#endif

module PeaZip.ShellExt:RootCommand;

import std;
import winrt.Windows.Foundation;
import winrt.Microsoft.Windows.Storage;

import :Resources;
import :Catalog;
import :EnumExplorerCommand;

using namespace winrt::Microsoft::Windows::Storage;
using namespace PeaZip::ShellExt::EnumExplorerCommand;

namespace PeaZip::ShellExt::RootCommand
{
constexpr std::wstring_view DEFAULT_SUB_COMMANDS =
    L"PeaZip.ext2main;PeaZip.ext2here;PeaZip.ext2smart;PeaZip.ext2folder;PeaZip.ext2browseasarchive;"
    L"PeaZip.ext2browsepath;PeaZip.add2separate;PeaZip.add2separate7z;PeaZip.add2separatezip;"
    L"PeaZip.add2separate7zencrypt;PeaZip.add2convert;PeaZip.analyze";

winrt::hstring GetSubCommandsString() noexcept
{
    // 順序 1: Microsoft.Windows.Storage.ApplicationData.LocalSettings
    try
    {
        auto localSettings = ApplicationData::GetDefault().LocalSettings();
        if (auto boxed = localSettings.Values().TryLookup(L"SubCommands"))
        {
            return winrt::unbox_value<winrt::hstring>(boxed);
        }
    }
    catch (...)
    {
    }

#pragma region ReadRegistrySubCommands
    wil::unique_cotaskmem_string regValue;
    if (SUCCEEDED(
            wil::reg::get_value_string_nothrow(HKEY_CLASSES_ROOT, LR"(*\shell\PeaZip)", L"SubCommands", regValue)))
    {
        const wchar_t* pRegStr = regValue.get();
        if (pRegStr[0] != L'\0')
        {
            return winrt::hstring(pRegStr);
        }
    }
#pragma endregion

    // 順序 3: フォールバック値
    return winrt::hstring{DEFAULT_SUB_COMMANDS};
}

IFACEMETHODIMP RootExploreCommand::GetTitle(IShellItemArray*, LPWSTR* ppszName) noexcept
{
    if (ppszName == nullptr) [[unlikely]]
    {
        return E_POINTER;
    }
    *ppszName = nullptr;

    auto title = wil::make_cotaskmem_string_nothrow(Title.data(), Title.size());
    if (!title) [[unlikely]]
    {
        return E_OUTOFMEMORY;
    }

    *ppszName = title.release();
    return S_OK;
}

IFACEMETHODIMP RootExploreCommand::GetIcon(IShellItemArray*, LPWSTR* ppszIcon) noexcept
{
    if (ppszIcon == nullptr) [[unlikely]]
    {
        return E_POINTER;
    }
    *ppszIcon = nullptr;

    auto iconPath =
        wil::make_cotaskmem_string_nothrow(Resources::MenuIconMain.data(), Resources::MenuIconMain.length());
    if (!iconPath)
    {
        return E_OUTOFMEMORY;
    }

    *ppszIcon = iconPath.release();
    return S_OK;
}

IFACEMETHODIMP RootExploreCommand::GetToolTip(IShellItemArray*, LPWSTR* ppszInfotip) noexcept
{
    *ppszInfotip = nullptr;
    return E_NOTIMPL;
}

IFACEMETHODIMP RootExploreCommand::GetState(IShellItemArray*, BOOL, EXPCMDSTATE* pCmdState) noexcept
{
    *pCmdState = ECS_ENABLED;
    return S_OK;
}

IFACEMETHODIMP RootExploreCommand::Invoke(IShellItemArray*, IBindCtx*) noexcept
{
    return S_OK;
}

IFACEMETHODIMP RootExploreCommand::GetFlags(EXPCMDFLAGS* pFlags) noexcept
{
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

IFACEMETHODIMP RootExploreCommand::EnumSubCommands(IEnumExplorerCommand** ppEnum) noexcept
{
    if (ppEnum == nullptr)
    {
        return E_POINTER;
    }
    *ppEnum = nullptr;

    try
    {
        winrt::hstring rawConfig = GetSubCommandsString();

#pragma region CreateSubCommands
        std::vector<winrt::com_ptr<IExplorerCommand>> commands;
        for (auto&& itemRange : std::wstring_view(rawConfig) | std::views::split(L';'))
        {
            if (std::wstring_view id(itemRange); !id.empty())
            {
                if (auto cmd = Catalog::CreateCommand(id); cmd != nullptr)
                {
                    commands.push_back(std::move(cmd));
                }
            }
        }
#pragma endregion

        // 3. “Move” the vector to generate an enumerator, and pass it directly to the explorer across the ABI boundary
        winrt::make<CommandEnumerator>(std::move(commands)).copy_to(ppEnum);

        return S_OK;
    }
    catch (...)
    {
        return winrt::to_hresult();
    }
}

} // namespace PeaZip::ShellExt::RootCommand
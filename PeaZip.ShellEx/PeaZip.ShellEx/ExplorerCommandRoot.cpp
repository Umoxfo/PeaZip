/*
 * This file is part of PeaZip.
 *
 * PeaZip is free software: you can redistribute it and/or modify it under the terms of
 * the GNU Lesser General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * PeaZip is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with PeaZip.
 * If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * PROJECT:   PeaZip
 * FILE:      ExplorerCommandRoot.cpp
 * PURPOSE:   Implementation of the root command for PeaZip context menu
 *            (for files and directories)
 *
 * LICENSE:   LGPL-3
 *
 * DEVELOPER: Makoto Sakaguchi (ycco34vx@gmail.com)
 */

#include "pch.h"
#include "ExplorerCommandRoot.h"

import std;
import util.registry;
import util.icon;
import config;

using namespace winrt;
using namespace Windows::Storage;

ExplorerCommandRoot::ExplorerCommandRoot()
{
    static const auto localSettings{ ApplicationData::Current().LocalSettings() };

    auto composite{ localSettings.Values().Lookup(L"contextMenu").as<ApplicationDataCompositeValue>() };
    if (composite == nullptr)
    {
        load_from_registry(m_commands);
    }
    else
    {
        load_from_application_data(composite, m_commands);
    }

    m_current = m_commands.cbegin();

    m_initialized = true;
}

#pragma region IExplorerCommand
HRESULT __stdcall ExplorerCommandRoot::GetTitle(IShellItemArray*, LPWSTR* ppszName) noexcept
{
    return SHStrDupW(L"PeaZip", ppszName);
}

HRESULT __stdcall ExplorerCommandRoot::GetIcon(IShellItemArray*, LPWSTR* ppszIcon) noexcept
{
    return SHStrDupW(g_main_app_icon_path.c_str(), ppszIcon);
}

HRESULT __stdcall ExplorerCommandRoot::GetToolTip(IShellItemArray*, LPWSTR* ppszInfotip) noexcept
{
    *ppszInfotip = nullptr;
    return E_NOTIMPL;
}

HRESULT __stdcall ExplorerCommandRoot::GetCanonicalName(GUID* pguidCommandName) noexcept
{
    *pguidCommandName = __uuidof(this);
    return S_OK;
}

HRESULT __stdcall ExplorerCommandRoot::GetState(IShellItemArray*, BOOL, EXPCMDSTATE* pCmdState) noexcept
{
    *pCmdState = ECS_ENABLED;
    return S_OK;
}

HRESULT __stdcall ExplorerCommandRoot::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc) noexcept
{
    UNREFERENCED_PARAMETER(psiItemArray);
    UNREFERENCED_PARAMETER(pbc);
    return E_NOTIMPL;
}

HRESULT __stdcall ExplorerCommandRoot::GetFlags(EXPCMDFLAGS* pFlags) noexcept
{
    *pFlags = ECF_HASSUBCOMMANDS;
    return S_OK;
}

HRESULT __stdcall ExplorerCommandRoot::EnumSubCommands(IEnumExplorerCommand** ppEnum) noexcept
{
    m_current = m_commands.cbegin();
    AddRef();
    return QueryInterface(IID_PPV_ARGS(ppEnum));
}
#pragma endregion

#pragma region IEnumExplorerCommand
HRESULT __stdcall ExplorerCommandRoot::Next(
    _In_ ULONG celt,
    _Out_writes_to_(celt, *pceltFetched) IExplorerCommand** pUICommand,
    _Out_opt_ ULONG* pceltFetched) noexcept
{
    ULONG fetched = 0;
    for (ULONG i = 0; (i < celt) && (m_current != m_commands.cend()); i++)
    {
        m_current->copy_to(&pUICommand[i]);
        m_current++;
        fetched++;
    }

    wil::assign_to_opt_param(pceltFetched, fetched);
    return (fetched == celt) ? S_OK : S_FALSE;
}

HRESULT __stdcall ExplorerCommandRoot::Skip(ULONG) noexcept { return E_NOTIMPL; }

HRESULT __stdcall ExplorerCommandRoot::Reset(void) noexcept
{
    m_current = m_commands.cbegin();
    return S_OK;
}

HRESULT __stdcall ExplorerCommandRoot::Clone(IEnumExplorerCommand** ppenum) noexcept
{
    *ppenum = nullptr;
    return E_NOTIMPL;
}
#pragma endregion

module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

module PeaZip.ShellExt:EnumExplorerCommand;

import std;
import winrt_base;

namespace PeaZip::ShellExt::EnumExplorerCommand
{
HRESULT STDMETHODCALLTYPE
CommandEnumerator::Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched) noexcept
{
    // Initialize the fetched counter if provided
    if (pceltFetched != nullptr)
    {
        *pceltFetched = 0;
    }

    // Validate arguments
    if (pUICommand == nullptr)
    {
        return E_POINTER;
    }

    // Calculate how many items we can actually fetch
    size_t remaining = m_commands.size() - m_index;
    auto items_to_fetch = (std::min)(static_cast<size_t>(celt), remaining);

    // Populate the output array using the pre-calculated count
    for (size_t i = 0; i < items_to_fetch; ++i)
    {
        // Copy the com_ptr and increment the reference count for the caller
        m_commands[m_index].copy_to(&pUICommand[i]);
        m_index++;
    }

    // Set the number of items successfully fetched
    if (pceltFetched != nullptr)
    {
        *pceltFetched = static_cast<ULONG>(items_to_fetch);
    }

    // Return S_OK if all requested elements were fetched.
    // Return S_FALSE if fewer elements were retrieved due to reaching the end.
    return (items_to_fetch == celt) ? S_OK : S_FALSE;
}
} // namespace PeaZip::ShellExt::EnumExplorerCommand
module;
#include <shobjidl_core.h>

export module PeaZip.ShellExt:EnumExplorerCommand;

import std;
import winrt_base;

namespace PeaZip::ShellExt::EnumExplorerCommand
{
export struct CommandEnumerator : winrt::implements<CommandEnumerator, IEnumExplorerCommand>
{
    explicit CommandEnumerator(std::vector<winrt::com_ptr<IExplorerCommand>>&& commands)
        : m_commands(std::move(commands))
    {
    }

    HRESULT STDMETHODCALLTYPE Next(
        _In_ ULONG celt,
        _Out_writes_to_(celt, *pceltFetched) IExplorerCommand** pUICommand,
        _Out_opt_ _Deref_out_range_(0, celt) ULONG* pceltFetched) noexcept override;

    HRESULT STDMETHODCALLTYPE Skip(_In_ ULONG celt) noexcept override
    {
        const auto previous_index = m_index;

        // Advance the index, clamping it to the container's maximum size
        m_index = (std::min)(m_index + static_cast<size_t>(celt), m_commands.size());

        // Return S_OK only if the index advanced by the exact requested amount (celt)
        return (m_index - previous_index == celt) ? S_OK : S_FALSE;
    }

    HRESULT STDMETHODCALLTYPE Reset(void) noexcept override
    {
        m_index = 0;
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE Clone(_COM_Outptr_ IEnumExplorerCommand** ppenum) noexcept override
    {
        *ppenum = nullptr;
        return E_NOTIMPL;
    }

private:
    std::vector<winrt::com_ptr<IExplorerCommand>> m_commands;
    size_t m_index{0};
};
} // namespace PeaZip::ShellExt::EnumExplorerCommand
module;
#include <shobjidl_core.h>

export module PeaZip.ShellExt:SubCommand;

import std;
import winrt_base;

import :types;

export namespace PeaZip::ShellExt::SubCommand
{
class GenericSubCommand : public winrt::implements<GenericSubCommand, IExplorerCommand>
{
public:
    /// <summary>
    /// Initializes a new instance of <see cref="GenericSubCommand"/>.
    /// Accepts metadata by value and moves it to prioritize simplicity and safety.
    /// </summary>
    /// <param name="metadata">Immutable command metadata.</param>
    explicit GenericSubCommand(CommandMetadata metadata) noexcept : m_metadata(std::move(metadata))
    {
    }

    IFACEMETHODIMP GetTitle(
        _In_opt_ IShellItemArray* psiItemArray, _Outptr_result_nullonfailure_ LPWSTR* ppszName) noexcept override;
    IFACEMETHODIMP GetIcon(
        _In_opt_ IShellItemArray* psiItemArray, _Outptr_result_nullonfailure_ LPWSTR* ppszIcon) noexcept override;

    IFACEMETHODIMP GetToolTip(
        [[maybe_unused]] _In_opt_ IShellItemArray* psiItemArray,
        _Outptr_result_nullonfailure_ LPWSTR* ppszInfotip) noexcept override
    {
        *ppszInfotip = nullptr;
        return E_NOTIMPL;
    }

    IFACEMETHODIMP GetCanonicalName(_Out_ GUID* pguidCommandName) noexcept override
    {
        if (pguidCommandName == nullptr)
        {
            return E_INVALIDARG;
        }

        *pguidCommandName = GUID_NULL;
        return S_OK;
    }

    IFACEMETHODIMP GetState(
        [[maybe_unused]] _In_opt_ IShellItemArray* psiItemArray,
        [[maybe_unused]] _In_ BOOL fOkToBeSlow,
        _Out_ EXPCMDSTATE* pCmdState) noexcept override
    {
        *pCmdState = ECS_ENABLED;
        return S_OK;
    }

    IFACEMETHODIMP Invoke(_In_opt_ IShellItemArray* psiItemArray, _In_opt_ IBindCtx* pbc) noexcept override;

    IFACEMETHODIMP GetFlags(_Out_ EXPCMDFLAGS* pFlags) noexcept override
    {
        *pFlags = m_metadata.flags;
        return S_OK;
    }

    IFACEMETHODIMP EnumSubCommands(_COM_Outptr_ IEnumExplorerCommand** ppEnum) noexcept override
    {
        *ppEnum = nullptr;
        return E_NOTIMPL;
    }

private:
    CommandMetadata m_metadata;
    winrt::hstring m_cachedTitle;

    std::vector<std::wstring> ExtractFilePaths(IShellItemArray* shellItemArray) const noexcept;
};
} // namespace PeaZip::ShellExt::SubCommand
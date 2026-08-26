module;
#include "pch.h"

export module PeaZip.ShellExt:GenericCommand;

import std;
import winrt_base;

import :types;

export namespace PeaZip::ShellExt
{
class GenericExplorerCommand : public winrt::implements<GenericExplorerCommand, IExplorerCommand>
{
public:
    /// <summary>
    /// Initializes a new instance of <see cref="GenericExplorerCommand"/>.
    /// Accepts metadata by value and moves it to prioritize simplicity and safety.
    /// </summary>
    /// <param name="metadata">Immutable command metadata.</param>
    explicit GenericExplorerCommand(CommandMetadata metadata) noexcept;

    IFACEMETHODIMP GetTitle(_In_opt_ IShellItemArray* psiItemArray,
                            _Outptr_result_nullonfailure_ LPWSTR* ppszName) noexcept override;
    IFACEMETHODIMP GetIcon(_In_opt_ IShellItemArray* psiItemArray,
                           _Outptr_result_nullonfailure_ LPWSTR* ppszIcon) noexcept override;
    IFACEMETHODIMP GetToolTip(_In_opt_ IShellItemArray* psiItemArray,
                              _Outptr_result_nullonfailure_ LPWSTR* ppszInfotip) noexcept override;
    IFACEMETHODIMP GetCanonicalName(_Out_ GUID* pguidCommandName) noexcept override;
    IFACEMETHODIMP GetState(_In_opt_ IShellItemArray* psiItemArray, _In_ BOOL fOkToBeSlow,
                            _Out_ EXPCMDSTATE* pCmdState) noexcept override;
    IFACEMETHODIMP Invoke(_In_opt_ IShellItemArray* psiItemArray, _In_opt_ IBindCtx* pbc) noexcept override;
    IFACEMETHODIMP GetFlags(_Out_ EXPCMDFLAGS* pFlags) noexcept override;
    IFACEMETHODIMP EnumSubCommands(_Outptr_opt_ IEnumExplorerCommand** ppEnum) noexcept override;

private:
    CommandMetadata m_metadata;
    winrt::hstring m_cachedTitle;

    std::vector<std::wstring> ExtractFilePaths(winrt::com_ptr<IShellItemArray> const& shellItemArray) const noexcept;
};
} // namespace PeaZip::ShellExt
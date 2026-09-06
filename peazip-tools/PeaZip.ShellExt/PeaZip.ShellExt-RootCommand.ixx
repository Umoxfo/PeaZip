module;
#include <shobjidl_core.h>

export module PeaZip.ShellExt:RootCommand;

import std;
import winrt_base;

export namespace PeaZip::ShellExt::RootCommand
{
struct __declspec(uuid("8EFA1ABF-C22C-456E-BD81-373A74D6107D")) RootExploreCommand
    : winrt::implements<RootExploreCommand, IExplorerCommand>
{
    IFACEMETHODIMP GetTitle(
        _In_opt_ IShellItemArray* psiItemArray, _Outptr_result_nullonfailure_ LPWSTR* ppszName) noexcept override;
    IFACEMETHODIMP GetIcon(
        _In_opt_ IShellItemArray* psiItemArray, _Outptr_result_nullonfailure_ LPWSTR* ppszIcon) noexcept override;
    IFACEMETHODIMP GetToolTip(
        _In_opt_ IShellItemArray* psiItemArray, _Outptr_result_nullonfailure_ LPWSTR* ppszInfotip) noexcept override;

    IFACEMETHODIMP GetCanonicalName(_Out_ GUID* pguidCommandName) noexcept override
    {
        *pguidCommandName = __uuidof(RootExploreCommand);
        return S_OK;
    }

    IFACEMETHODIMP
    GetState(
        _In_opt_ IShellItemArray* psiItemArray, _In_ BOOL fOkToBeSlow, _Out_ EXPCMDSTATE* pCmdState) noexcept override;
    IFACEMETHODIMP Invoke(_In_opt_ IShellItemArray* psiItemArray, _In_opt_ IBindCtx* pbc) noexcept override;
    IFACEMETHODIMP GetFlags(_Out_ EXPCMDFLAGS* pFlags) noexcept override;
    IFACEMETHODIMP EnumSubCommands(_COM_Outptr_ IEnumExplorerCommand** ppEnum) noexcept override;

private:
    static constexpr std::wstring_view Title = L"PeaZip";
};

constexpr GUID CLSID_RootExploreCommand = __uuidof(RootExploreCommand);

} // namespace PeaZip::ShellExt::RootCommand
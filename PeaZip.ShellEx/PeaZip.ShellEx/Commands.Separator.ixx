module;
#include "pch.h"

export module Commands.Separator;

using namespace winrt;

export struct Separator : implements<Separator, IExplorerCommand>
{
    HRESULT __stdcall GetTitle(IShellItemArray*, LPWSTR* ppszName) noexcept override
    {
        *ppszName = nullptr;
        return S_FALSE;
    }

    HRESULT __stdcall GetIcon(IShellItemArray*, LPWSTR* ppszIcon) noexcept override
    {
        *ppszIcon = nullptr;
        return E_NOTIMPL;
    }

    HRESULT __stdcall GetToolTip(IShellItemArray*, LPWSTR* ppszInfotip) noexcept override
    {
        *ppszInfotip = nullptr;
        return E_NOTIMPL;
    }

    HRESULT __stdcall GetCanonicalName(GUID* pguidCommandName) noexcept override
    {
        *pguidCommandName = GUID_NULL;
        return E_NOTIMPL;
    }

    HRESULT __stdcall GetState(IShellItemArray*, BOOL, EXPCMDSTATE* pCmdState) noexcept override
    {
        *pCmdState = ECS_ENABLED;
        return S_OK;
    }

    HRESULT __stdcall Invoke(IShellItemArray*, IBindCtx*) noexcept override { return S_OK; }

    HRESULT __stdcall GetFlags(EXPCMDFLAGS* pFlags) noexcept override
    {
        *pFlags = ECF_ISSEPARATOR;
        return S_OK;
    }

    HRESULT __stdcall EnumSubCommands(IEnumExplorerCommand** ppEnum) noexcept override
    {
        *ppEnum = nullptr;
        return E_NOTIMPL;
    }
};

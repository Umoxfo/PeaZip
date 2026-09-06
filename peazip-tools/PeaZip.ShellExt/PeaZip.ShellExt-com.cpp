module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

module PeaZip.ShellExt:com;

import std;
import winrt_base;

import :RootCommand;

using namespace PeaZip::ShellExt::RootCommand;

namespace
{
template <typename T>
struct GenericClassFactory : winrt::implements<GenericClassFactory<T>, IClassFactory>
{
    HRESULT STDMETHODCALLTYPE
    CreateInstance(_In_opt_ IUnknown*, _In_ REFIID riid, _COM_Outptr_ void** ppv) noexcept override
    {
        if (ppv == nullptr)
        {
            return E_POINTER;
        }
        *ppv = nullptr;

        try
        {
            return winrt::make<T>().as(riid, ppv);
        }
        catch (...)
        {
            return winrt::to_hresult();
        }
    }

    HRESULT STDMETHODCALLTYPE LockServer(_In_ BOOL fLock) noexcept override
    {
        if (fLock)
        {
            ++winrt::get_module_lock();
        }
        else
        {
            --winrt::get_module_lock();
        }

        return S_OK;
    }
};
} // namespace

HRESULT __stdcall PeaZip::ShellExt::com::DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) noexcept
{
    if (ppv == nullptr)
    {
        return E_POINTER;
    }
    *ppv = nullptr;

    try
    {
        if (rclsid == CLSID_RootExploreCommand)
        {
            return winrt::make<GenericClassFactory<RootExploreCommand>>().as(riid, ppv);
        }
        // else if (rclsid == CLSID_RootExploreCommand)
        //{
        // }
    }
    catch (...)
    {
        return winrt::to_hresult();
    }

    return CLASS_E_CLASSNOTAVAILABLE;
}

HRESULT __stdcall PeaZip::ShellExt::com::DllCanUnloadNow() noexcept
{
    return (winrt::get_module_lock() == 0) ? S_OK : S_FALSE;
}
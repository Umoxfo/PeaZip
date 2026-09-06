module;
#include "pch.h"

export module PeaZip.ShellExt:com;

import std;

export namespace PeaZip::ShellExt::com
{
HRESULT __stdcall DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _COM_Outptr_ LPVOID* ppv) noexcept;
HRESULT __stdcall DllCanUnloadNow() noexcept;
} // namespace PeaZip::ShellExt::com
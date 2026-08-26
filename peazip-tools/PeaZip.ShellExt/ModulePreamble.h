#pragma once

// Import the C++/WinRT namespaces used across this project
import winrt_base;
import winrt.Windows.Foundation;
import winrt.Windows.Foundation.Collections;

#define WINRT_IMPORT_MODULE
// Define header guards to light up WIL's conditional winrt features.
// You can either define the guards directly, or include the now-inert
// winrt headers (which define the guards as a side effect):
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <wil/cppwinrt.h>
#include <wil/cppwinrt_helpers.h>

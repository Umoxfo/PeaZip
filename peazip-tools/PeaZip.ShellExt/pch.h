#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shobjidl_core.h>
#include <shellapi.h>

#include <wil/stl.h>
#include <wil/resource.h>

// Pre-include STL headers that other libraries include after import std;
#include <format>
#include <latch>
#include <ranges>
#include <variant>
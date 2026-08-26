module;
#include "pch.h"
#include <flat_map>

export module PeaZip.ShellExt:Catalog;

import std;
import winrt_base;

import :types;

namespace PeaZip::ShellExt
{
// ============================================================================
// Command Metadata Catalog Initialization
// Data extracted from PeaZip Inno Setup Script (peazip-setup_script-configure.iss)
// ============================================================================
constexpr std::pair<std::wstring_view, CommandMetadata> c_catalogEntries[] = {
#include "src/generated/GeneratedCommands.inc"
};

/// <summary>
/// Retrieves the static singleton instance of the command catalog.
/// </summary>
/// <remarks>
/// Uses <see cref="std::flat_map"/> for cache-friendly, zero-allocation lookups.
/// </remarks>
CommandCatalog const& GetCatalog() noexcept
{ // Thread-safe lazy initialization of std::flat_map using C++23 std::from_range
    static const CommandCatalog s_catalog{std::from_range, c_catalogEntries};
    return s_catalog;
} // namespace PeaZip::ShellExt

export winrt::com_ptr<IExplorerCommand> CreateCommand(std::wstring_view commandId) noexcept;
} // namespace PeaZip::ShellExt
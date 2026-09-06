module;
#include "pch.h"

#ifdef __INTELLISENSE__
#include <winrt/base.h>
#endif

module PeaZip.ShellExt:Catalog;

import std;
import winrt_base;

import :types;
import :SubCommand;

namespace PeaZip::ShellExt::Catalog
{
using CommandCatalog = std::flat_map<std::wstring_view, CommandMetadata, std::less<>>;

// ============================================================================
// Command Metadata Catalog Initialization
// Data extracted from PeaZip Inno Setup Script (peazip-setup_script-configure.iss)
// ============================================================================
constexpr std::pair<std::wstring_view, CommandMetadata> c_catalogEntries[] = {
#include "src/generated/gen_command_catalog.inc"
};

/// <summary>
/// Retrieves the static singleton instance of the command catalog.
/// </summary>
/// <remarks>Uses <see cref="std::flat_map"/> for cache-friendly, zero-allocation lookups.</remarks>
CommandCatalog const& GetCatalog() noexcept
{
    // Thread-safe lazy initialization of std::flat_map using C++23 std::from_range
    static const CommandCatalog s_catalog{std::from_range, c_catalogEntries};
    return s_catalog;
}

winrt::com_ptr<IExplorerCommand> CreateCommand(std::wstring_view commandId) noexcept
{
    try
    {
        auto const& catalog = GetCatalog();

        // 1. Look up the command ID in the pre-defined catalog
        if (auto it = catalog.find(commandId); it != catalog.end())
        {

            // 2. Instantiate GenericSubCommand with the found metadata
            return winrt::make<SubCommand::GenericSubCommand>(it->second).as<IExplorerCommand>();
        }
    }
    catch (...)
    {
        // Catch std::bad_alloc or WinRT exceptions to prevent ABI boundary crashes.
        // Returning nullptr safely indicates failure to the caller.
        return nullptr;
    }

    // Return nullptr if the commandId is unknown
    return nullptr;
}

} // namespace PeaZip::ShellExt::Catalog
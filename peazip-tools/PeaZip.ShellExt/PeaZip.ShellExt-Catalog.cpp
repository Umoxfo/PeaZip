module;
#include "pch.h"

module PeaZip.ShellExt:Catalog;

import std;
import winrt_base;

import :types;
import :GenericCommand;

namespace PeaZip::ShellExt
{
inline std::flat_map<std::wstring_view, CommandMetadata, std::less<void>> const& GetCatalog()
{
    static CommandCatalog const catalog(std::sorted_unique, {
#include "src/generated/GeneratedCommands.inc"
                                                            });

    return catalog;
}

winrt::com_ptr<IExplorerCommand> CreateCommand(std::wstring_view commandId) noexcept
{
    auto const& catalog = GetCatalog();

    if (auto it = catalog.find(commandId); it != catalog.end())
    {
        return winrt::make<GenericExplorerCommand>(it->second);
    }
    return nullptr;
}
} // namespace PeaZip::ShellExt
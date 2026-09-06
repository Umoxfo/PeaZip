module;
#include <shobjidl_core.h>

export module PeaZip.ShellExt:Catalog;

import std;
import winrt_base;

namespace PeaZip::ShellExt::Catalog
{
/// <summary>
/// Creates and returns an instance of <see cref="GenericSubCommand"/> for the given command ID.
/// </summary>
/// <param name="commandId">The CommandStore ID (e.g., L"PeaZip.add2separate")</param>
/// <returns>A COM pointer to the instantiated command, or nullptr if not found.</returns>
export winrt::com_ptr<IExplorerCommand> CreateCommand(std::wstring_view commandId) noexcept;
} // namespace PeaZip::ShellExt::Catalog
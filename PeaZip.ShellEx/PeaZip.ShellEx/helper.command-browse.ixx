module;
#include "pch.h"
#include "ExplorerCommandRoot.h"
#include "ExplorerCommandBase.h"
#include "BrowsePathCommand.h"

#pragma warning(suppress:5201)
export module helper.command:browse;

import util.icon;

using namespace winrt;

export inline void add_browse_archive_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"OpenArchive/Content", g_main_app_icon_path, L"-ext2browseasarchive "));
}

export inline void add_browse_path_command(SubCommandList& commands)
{
    commands.push_back(make<BrowsePathCommand>());
}

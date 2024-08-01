module;
#include "pch.h"
#include "ExplorerCommandRoot.h"
#include "ExplorerCommandBase.h"

#pragma warning(suppress:5201)
export module helper.command:extract;

import util.icon;

using namespace winrt;

export inline void add_extract_main_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"Ext/Content", g_extract_icon_path, L"-ext2multi "));
}

export inline void add_extract_here_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"ExtHere/Content", g_extract_icon_path, L"-ext2multihere "));
}

export inline void add_extract_smart_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"ExtSmart/Content", g_extract_folder_icon_path, L"-ext2multismart "));
}

export inline void add_extract_folder_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"ExtFolder/Content", g_extract_folder_icon_path, L"-ext2multifolder "));
}

export inline void add_extract_test_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"Test/Content", g_test_icon_path, L"-ext2test "));
}

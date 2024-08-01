module;
#include "pch.h"
#include "ExplorerCommandRoot.h"

#pragma warning(suppress:5201)
export module helper.command:util;

import Commands.Analyze;
import util.icon;

using namespace winrt;

export inline void add_analyze_command(SubCommandList& commands)
{
    commands.push_back(make<AnalyzeCommand>());
}

export inline void add_wipe_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"SecureDelete/Content", g_delete_icon_path, L"-add2wipe "));
}

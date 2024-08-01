module;
#include "pch.h"
#include "ExplorerCommandRoot.h"
#include "ExplorerCommandBase.h"

#pragma warning(suppress:5201)
export module helper.command:add;

import util.icon;

using namespace winrt;

export inline void add_archive_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddArchive/Content", g_add_archive_icon_path, L"-add2multi "));
}

export inline void add_separate_archive_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddToSeparateArchive/Content", g_add_archive_icon_path, L"-add2multiseparate "));
}

export inline void add_sfx_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddToSelfExtracting/Content", g_add_sfx_icon_path, L"-add2multisfx "));
}

export inline void add_7z_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"Add7z/Content", g_add_7z_icon_path, L"-add2multi7z "));
}

export inline void add_gz_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddGZip/Content", g_add_archive_icon_path, L"-add2multigz "));
}

export inline void add_xz_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddXz/Content", g_add_archive_icon_path, L"-add2multixz "));
}

export inline void add_zip_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddZip/Content", g_add_zip_icon_path, L"-add2multizip "));
}

export inline void add_zstd_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddZstd/Content", g_add_archive_icon_path, L"-add2multizstd "));
}

export inline void add_7z_encrypt_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"Encrypt7z/Content", g_lock_icon_path, L"-add2multi7zencrypt "));
}

export inline void add_zip_mail_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"AddToZipAndMail/Content", g_mail_icon_path, L"-add2multizipmail "));
}

export inline void add_split_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"SplitFile/Content", g_sprit_icon_path, L"-add2multisplit "));
}

export inline void add_convert_command(SubCommandList& commands)
{
    commands.push_back(make<ExplorerCommandBase>(L"Convert/Content", g_convert_icon_path, L"-add2multiconvert "));
}

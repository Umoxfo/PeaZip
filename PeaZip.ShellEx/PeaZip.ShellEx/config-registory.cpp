module;
#include "pch.h"
#include "ExplorerCommandRoot.h"

#pragma warning(suppress:5201)
module config;

import std;
import util.registry;
import util.icon;
import Commands.Separator;
import helper.command;

using namespace winrt;

static const std::unordered_map<std::wstring, void(*)(SubCommandList&)> extract_page_id_map =
{
    { L"PeaZip.ext2main", add_extract_main_command },
    { L"PeaZip.ext2here", add_extract_here_command },
    { L"PeaZip.ext2smart", add_extract_smart_command },
    { L"PeaZip.ext2folder", add_extract_folder_command },
    { L"PeaZip.ext2test", add_extract_test_command }
};

static const std::unordered_map<std::wstring, void(*)(SubCommandList&)> browse_page_id_map =
{
    { L"PeaZip.ext2browseasarchive", add_browse_archive_command },
    { L"PeaZip.ext2browsepath", add_browse_path_command }
};

static const std::unordered_map<std::wstring, void(*)(SubCommandList&)> add_page_id_map =
{
    { L"PeaZip.add2separate", add_archive_command },
    { L"PeaZip.add2separatesingle", add_separate_archive_command },
    { L"PeaZip.add2separatesfx", add_sfx_command },
    { L"PeaZip.add2separate7z", add_7z_command },
    { L"PeaZip.add2separategz", add_gz_command },
    { L"PeaZip.add2separatexz", add_xz_command },
    { L"PeaZip.add2separatezip", add_zip_command },
    { L"PeaZip.add2separatezstd", add_zstd_command },
    { L"PeaZip.add2separate7zencrypt", add_7z_encrypt_command },
    { L"PeaZip.add2separatezipmail", add_zip_mail_command },
    { L"PeaZip.add2split", add_split_command },
    { L"PeaZip.add2convert", add_convert_command }
};

static const std::unordered_map<std::wstring, void(*)(SubCommandList&)> util_page_id_map =
{
    { L"PeaZip.analyze", add_analyze_command },
    { L"PeaZip.add2wipe", add_wipe_command }
};

static inline void load_default(SubCommandList& commands)
{
    commands.reserve(15);
    add_extract_main_command(commands);
    add_extract_here_command(commands);
    add_extract_smart_command(commands);
    add_extract_folder_command(commands);
    commands.push_back(make<Separator>());
    add_browse_archive_command(commands);
    add_browse_path_command(commands);
    commands.push_back(make<Separator>());
    add_archive_command(commands);
    add_7z_command(commands);
    add_zip_command(commands);
    add_7z_encrypt_command(commands);
    add_convert_command(commands);
    commands.push_back(make<Separator>());
    add_analyze_command(commands);
}

void load_from_registry(SubCommandList& commands)
{
    static const auto add_separator = [&](bool& addedSeparator, bool& needSeparator)
        {
            if (!addedSeparator && needSeparator)
            {
                commands.push_back(make<Separator>());

                addedSeparator = true;
                needSeparator = false;
            }
        };

    std::wstring subCommands;
    auto retCode = RegGetString(HKEY_CLASSES_ROOT, LR"(AllFilesystemObjects\shell\PeaZip)", L"SubCommands", subCommands);
    if (retCode == ERROR_SUCCESS)
    {
        commands.reserve(MAX_CONTEXT_MENU_ITEMS);

        std::erase(subCommands, L' ');
        std::wistringstream ss(subCommands);
        std::wstring command;
        bool addedSeparator = false;
        bool needSeparator = false;
        while (std::getline(ss, command, L';'))
        {
            if (extract_page_id_map.contains(command))
            {
                needSeparator = true;
                extract_page_id_map.at(command)(commands);
                continue;
            }

            add_separator(addedSeparator, needSeparator);

            if (browse_page_id_map.contains(command))
            {
                needSeparator = true;
                browse_page_id_map.at(command)(commands);
                continue;
            }
            else
            {
                addedSeparator = false;
            }

            add_separator(addedSeparator, needSeparator);

            if (add_page_id_map.contains(command))
            {
                needSeparator = true;
                add_page_id_map.at(command)(commands);
                continue;
            }
            else
            {
                addedSeparator = false;
            }

            add_separator(addedSeparator, needSeparator);

            if (util_page_id_map.contains(command))
            {
                util_page_id_map.at(command)(commands);
            }
        }

        commands.shrink_to_fit();
    }
    else
    {
        load_default(commands);
    }
}

module;
#include "pch.h"
#include "ExplorerCommandRoot.h"
#include <winrt/PeaZip.ShellEx.Flags.h>

#pragma warning(suppress:5201)
module config;

import std;
import helper.command;
import Commands.Separator;

using namespace winrt;
using namespace Windows::Storage;
using namespace PeaZip::ShellEx::Flags;

namespace Flags
{
    constexpr uint32_t None = 0x0;

#pragma region Extract
    constexpr uint32_t ExtractMain = static_cast<uint32_t>(Extract::ExtractMain);
    constexpr uint32_t ExtractHere = static_cast<uint32_t>(Extract::ExtractHere);
    constexpr uint32_t ExtractSmart = static_cast<uint32_t>(Extract::ExtractSmart);
    constexpr uint32_t ExtractFolder = static_cast<uint32_t>(Extract::ExtractFolder);
    constexpr uint32_t ExtractDesktop = static_cast<uint32_t>(Extract::ExtractDesktop);
    constexpr uint32_t ExtractDocument = static_cast<uint32_t>(Extract::ExtractDocument);
    constexpr uint32_t ExtractDownload = static_cast<uint32_t>(Extract::ExtractDownload);
    constexpr uint32_t ExtractTest = static_cast<uint32_t>(Extract::ExtractTest);
#pragma endregion

#pragma region Browse
    constexpr uint32_t BrowseArchive = static_cast<uint32_t>(Browse::BrowseArchive);
    constexpr uint32_t BrowsePath = static_cast<uint32_t>(Browse::BrowsePath);
#pragma endregion

#pragma region ArchiveFormat
    constexpr uint32_t AddArchive = static_cast<uint32_t>(ArchiveFormat::AddArchive);
    constexpr uint32_t AddSeparateArchive = static_cast<uint32_t>(ArchiveFormat::AddSeparateArchive);
    constexpr uint32_t AddSfx = static_cast<uint32_t>(ArchiveFormat::AddSfx);
    constexpr uint32_t Add7z = static_cast<uint32_t>(ArchiveFormat::Add7z);
    constexpr uint32_t AddBrotli = static_cast<uint32_t>(ArchiveFormat::AddBrotli);
    constexpr uint32_t AddBZip2 = static_cast<uint32_t>(ArchiveFormat::AddBZip2);
    constexpr uint32_t AddGz = static_cast<uint32_t>(ArchiveFormat::AddGz);
    constexpr uint32_t AddPea = static_cast<uint32_t>(ArchiveFormat::AddPea);
    constexpr uint32_t AddTar = static_cast<uint32_t>(ArchiveFormat::AddTar);
    constexpr uint32_t AddWim = static_cast<uint32_t>(ArchiveFormat::AddWim);
    constexpr uint32_t AddXz = static_cast<uint32_t>(ArchiveFormat::AddXz);
    constexpr uint32_t AddZip = static_cast<uint32_t>(ArchiveFormat::AddZip);
    constexpr uint32_t AddZqap = static_cast<uint32_t>(ArchiveFormat::AddZqap);
    constexpr uint32_t AddZstd = static_cast<uint32_t>(ArchiveFormat::AddZstd);
#pragma endregion

#pragma region ArchiveFormatFeature
    constexpr uint32_t Add7zEncrypt = static_cast<uint32_t>(ArchiveFormatFeature::Add7zEncrypt);
#pragma endregion

#pragma region ArchiveFeature
    constexpr uint32_t AddZipMail = static_cast<uint32_t>(ArchiveFeature::AddZipMail);
    constexpr uint32_t Split = static_cast<uint32_t>(ArchiveFeature::Split);
    constexpr uint32_t Convert = static_cast<uint32_t>(ArchiveFeature::Convert);
#pragma endregion

#pragma region Utils
    constexpr uint32_t Analyze = static_cast<uint32_t>(Utils::Analyze);
    constexpr uint32_t Wipe = static_cast<uint32_t>(Utils::Wipe);
#pragma endregion
}

inline void load_from_application_data(const ApplicationDataCompositeValue& composite, SubCommandList& commands)
{
    commands.reserve(MAX_CONTEXT_MENU_ITEMS);
    load_extract_section(composite, commands);
    load_browse_section(composite, commands);
    load_add_section(composite, commands);
    load_util_section(composite, commands);
    commands.shrink_to_fit();
}

void load_extract_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands)
{
    auto extract = unbox_value_or<uint32_t>(composite.TryLookup(L"extract"), Flags::None);
    if (extract == Flags::None) return;

    if (extract & Flags::ExtractMain)
    {
        add_extract_main_command(commands);
    }

    if (extract & Flags::ExtractHere)
    {
        add_extract_here_command(commands);
    }

    if (extract & Flags::ExtractSmart)
    {
        add_extract_smart_command(commands);
    }

    if (extract & Flags::ExtractFolder)
    {
        add_extract_folder_command(commands);
    }

    if (extract & Flags::ExtractTest)
    {
        add_extract_test_command(commands);
    }

    commands.push_back(make<Separator>());
}

void load_browse_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands)
{
    auto browse = unbox_value_or<uint32_t>(composite.Lookup(L"browse"), Flags::None);
    if (browse == Flags::None) return;

    if (browse & Flags::BrowseArchive)
    {
        add_browse_archive_command(commands);
    }

    if (browse & Flags::BrowsePath)
    {
        add_browse_path_command(commands);
    }

    commands.push_back(make<Separator>());
}

void load_add_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands)
{
    auto archiveFormat = unbox_value_or<uint32_t>(composite.Lookup(L"archiveFormat"), Flags::None);
    auto archiveFormatFeature = unbox_value_or<uint32_t>(composite.Lookup(L"archiveFormatFeature"), Flags::None);
    auto archiveFeature = unbox_value_or<uint32_t>(composite.Lookup(L"archiveFormatFeature"), Flags::None);
    if ((archiveFormat == Flags::None) && (archiveFormatFeature == Flags::None) && (archiveFeature == Flags::None)) return;

#pragma region Archive Format
    if (archiveFormat & Flags::AddArchive)
    {
        add_archive_command(commands);
    }

    if (archiveFormat & Flags::AddSeparateArchive)
    {
        add_separate_archive_command(commands);
    }

    if (archiveFormat & Flags::AddSfx)
    {
        add_sfx_command(commands);
    }

    if (archiveFormat & Flags::Add7z)
    {
        add_7z_command(commands);
    }

    if (archiveFormat & Flags::AddGz)
    {
        add_gz_command(commands);
    }

    if (archiveFormat & Flags::AddXz)
    {
        add_xz_command(commands);
    }

    if (archiveFormat & Flags::AddZip)
    {
        add_zip_command(commands);
    }

    if (archiveFormat & Flags::AddZstd)
    {
        add_zstd_command(commands);
    }

    if (archiveFormat & Flags::AddZstd)
    {
        add_zstd_command(commands);
    }
#pragma endregion

#pragma region Archive Format Feature
    if (archiveFormatFeature & Flags::Add7zEncrypt)
    {
        add_7z_encrypt_command(commands);
    }
#pragma endregion

#pragma region Archive Feature
    if (archiveFeature & Flags::AddZipMail)
    {
        add_zip_mail_command(commands);
    }

    if (archiveFeature & Flags::Split)
    {
        add_split_command(commands);
    }

    if (archiveFeature & Flags::Convert)
    {
        add_convert_command(commands);
    }
#pragma endregion

    commands.push_back(make<Separator>());
}

void load_util_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands)
{
    auto utils = unbox_value_or<uint32_t>(composite.Lookup(L"utils"), Flags::None);
    if (utils == Flags::None) return;

    if (utils & Flags::Analyze)
    {
        add_analyze_command(commands);
    }

    if (utils & Flags::Wipe)
    {
        add_wipe_command(commands);
    }
}

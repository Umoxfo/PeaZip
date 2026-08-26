module;
#include "pch.h"

export module PeaZip.ShellExt:types;

import std;

export namespace PeaZip::ShellExt
{
struct CommandMetadata
{
    const wchar_t* labelKey;        // Resource key for localization (e.g., L"Add2Separate_Label")
    const wchar_t* fallbackLabel;   // Fallback text if resource load fails
    const wchar_t* iconPath;        // Path to icon resource (e.g., LR"(res\share\icons\peazip_seven.icl)")
    int iconIndex;                  // Index of icon within resource (e.g., 2)
    EXPCMDFLAGS flags{ECF_DEFAULT}; // Windows SDK CommandStore flags
    const wchar_t* command;         // Execution command
    const wchar_t* arguments;       // Command arguments
};

using CommandCatalog = std::flat_map<std::wstring_view, CommandMetadata, std::less<>>;
} // namespace PeaZip::ShellExt
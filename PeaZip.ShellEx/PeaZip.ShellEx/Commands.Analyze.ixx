module;
#include "pch.h"
#include "ExplorerCommandBase.h"

#pragma warning(suppress:5201)
export module Commands.Analyze;

import std;
import util.icon;

export struct AnalyzeCommand : ExplorerCommandBase
{
    AnalyzeCommand() : ExplorerCommandBase(L"Analyze/Content", g_analyze_icon_path, L"PEAUTILS 12 ")
    {}

    ~AnalyzeCommand() {}

private:
    const wchar_t* application() const noexcept override final
    {
        static const std::wstring app{ L"pea.exe" };
        return app.c_str();
    }
};

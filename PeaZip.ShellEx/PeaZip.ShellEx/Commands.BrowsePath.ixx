module;
#include "pch.h"
#include "ExplorerCommandBase.h"

export module Commands.BrowsePath;

import util.icon;

export struct __declspec(uuid("01F6DED1-42C8-4CFA-9F2D-96998BA8BB17")) BrowsePathCommand : ExplorerCommandBase
{
    BrowsePathCommand() : ExplorerCommandBase(L"BrowsePath/Content", g_browse_path_icon_path, L"-ext2browsepath ") {}
    ~BrowsePathCommand() {};

    HRESULT __stdcall GetCanonicalName(GUID* pguidCommandName) noexcept override
    {
        *pguidCommandName = __uuidof(this);
        return S_OK;
    }
};

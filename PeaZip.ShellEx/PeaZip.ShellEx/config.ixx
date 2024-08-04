module;
#include "pch.h"
#include "ExplorerCommandRoot.h"

#pragma warning(suppress:5201)
export module config;

using namespace winrt::Windows::Storage;

inline constexpr int MAX_CONTEXT_MENU_ITEMS = 25;

export inline void load_from_application_data(const ApplicationDataCompositeValue& composite, SubCommandList& commands);
void load_extract_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands);
void load_browse_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands);
void load_add_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands);
void load_util_section(const ApplicationDataCompositeValue& composite, SubCommandList& commands);

export void load_from_registry(SubCommandList& commands);
inline void load_default(SubCommandList& commands);
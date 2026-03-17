#pragma once
#include <imgui.h>

namespace UI::Navigation {
    bool SearchBar(const char* id, char* buffer, size_t buffer_size, float width);
}
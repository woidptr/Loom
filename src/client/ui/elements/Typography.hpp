#pragma once
#include <imgui.h>

namespace UI::Elements::Typography {
    void DrawGradientText(ImDrawList* draw_list, ImVec2 pos, const char* text, ImU32 col_left, ImU32 col_right);
}
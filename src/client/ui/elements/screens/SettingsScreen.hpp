#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>

namespace UI::Elements::SettingsScreen {
    void DrawTitleBar(const ImGuiWindow* window, ImDrawList* draw_list, const std::function<void()>& on_window_close);
    void DrawNavBar(const ImGuiWindow* window, ImDrawList* draw_list);
}
#pragma once
#include <imgui.h>
#include <imgui_internal.h>
#include <functional>
#include <client/mods/Module.hpp>

namespace UI::Elements::SettingsScreen {
    void DrawTitleBar(const ImGuiWindow* window, ImDrawList* draw_list, const std::function<void()>& on_window_close);
    void DrawNavBar(const ImGuiWindow* window, ImDrawList* draw_list);
    bool DrawModCard(const ImGuiWindow* window, ImDrawList* draw_list, const char* id, const char* name, Module* module);
}
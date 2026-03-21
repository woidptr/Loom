#include "SettingsScreen.hpp"
#include <algorithm>
#include <client/ui/elements/Typography.hpp>
#include <client/ui/theme/FontManager.hpp>

namespace UI::Elements::SettingsScreen {
    void DrawTitleBar(const ImGuiWindow* window, ImDrawList* draw_list, const std::function<void()>& on_window_close) {
        const float titlebar_height = window->Size.y * 0.08f;
        const float padding_x = window->Size.x * 0.02f;

        const ImVec2 p_min = window->Pos;
        const ImVec2 p_max = ImVec2(p_min.x + window->Size.x, p_min.y + titlebar_height);

        draw_list->AddRectFilled(p_min, p_max, IM_COL32(10, 10, 10, 200), 5.f);

        draw_list->AddLine(
            ImVec2(p_min.x, p_max.y),
            ImVec2(p_max.x, p_max.y),
            IM_COL32(40, 40, 40, 200)
        );

        ImGui::PushFont(FontManager::montserrat_extra_bold);

        const float target_text_height = titlebar_height * 0.60f;
        const float scale = target_text_height / ImGui::GetTextLineHeight();

        ImGui::SetWindowFontScale(scale);

        const ImVec2 text_pos = ImVec2(
            p_min.x + padding_x,
            p_min.y + (titlebar_height - ImGui::GetTextLineHeight()) * 0.5f
        );
        constexpr ImU32 cyan = IM_COL32(34, 211, 238, 255);
        constexpr ImU32 blue = IM_COL32(59, 130, 246, 255);

        UI::Elements::Typography::DrawGradientText(draw_list, text_pos, "MYRA", cyan, blue);

        ImGui::SetWindowFontScale(1.f);
        ImGui::PopFont();

        const float btn_size = titlebar_height * 0.60f;
        const float cross_padding = btn_size * 0.28f;
        // float btn_rounding = btn_size * 0.15f;
        const float cross_thickness = std::max(1.0f, btn_size * 0.07f);

        const ImVec2 close_p_min = ImVec2(
            p_max.x - btn_size - padding_x,
            p_min.y + (titlebar_height - btn_size) * 0.5f
        );
        const ImVec2 close_p_max = ImVec2(close_p_min.x + btn_size, close_p_min.y + btn_size);

        ImGui::SetCursorScreenPos(close_p_min);
        if (ImGui::InvisibleButton("##close_btn", ImVec2(btn_size, btn_size))) {
            on_window_close();
        }

        const bool close_hovered = ImGui::IsItemHovered();
        if (close_hovered) {
            draw_list->AddRectFilled(close_p_min, close_p_max, IM_COL32(30, 41, 59, 255), 4.0f);
        }

        const ImU32 close_col = close_hovered ? IM_COL32(255, 255, 255, 255) : IM_COL32(148, 163, 184, 255);
        draw_list->AddLine(
            ImVec2(close_p_min.x + cross_padding, close_p_min.y + cross_padding),
            ImVec2(close_p_max.x - cross_padding, close_p_max.y - cross_padding),
            close_col, cross_thickness
        );
        draw_list->AddLine(
            ImVec2(close_p_max.x - cross_padding, close_p_min.y + cross_padding),
            ImVec2(close_p_min.x + cross_padding, close_p_max.y - cross_padding),
            close_col, cross_thickness
        );

        ImGui::SetCursorScreenPos(ImVec2(p_min.x, p_max.y));
    }

    void DrawNavBar(const ImGuiWindow* window, ImDrawList* draw_list) {
        const float navbar_height = window->Size.y * 0.07f;
        const float padding_x = window->Size.x * 0.02f;
        const float search_width = window->Size.x * 0.25f;

        ImVec2 start_pos = ImGui::GetCursorScreenPos();
        ImVec2 end_pos = ImVec2(start_pos.x + window->Size.x, start_pos.y + navbar_height);

        // draw_list->AddRectFilled(start_pos, end_pos, IM_COL32(2, 6, 23, 250));
        // draw_list->AddLine(ImVec2(start_pos.x, end_pos.y), ImVec2(end_pos.x, end_pos.y), IM_COL32(30, 41, 59, 255));

        // ImGui::SetCursorScreenPos(ImVec2(start_pos.x + 16.0f, start_pos.y + 10.0f));

        const float target_input_height = navbar_height * 0.60f;
        const float target_font_height = target_input_height * 0.65f;
        const float font_scale = std::min(1.f, target_font_height / ImGui::GetTextLineHeight());

        ImGui::SetWindowFontScale(font_scale);

        const float required_padding_y = std::max(
            0.f, (target_input_height - ImGui::GetTextLineHeight()) * 0.5f
        );
        const float center_y = start_pos.y + (navbar_height - target_input_height) * 0.5f;

        ImGui::SetCursorScreenPos(ImVec2(
            start_pos.x + window->Size.x - padding_x - search_width,
            center_y
        ));

        ImGui::PushItemWidth(search_width);

        ImVec2 current_padding = ImGui::GetStyle().FramePadding;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(current_padding.x, required_padding_y));

        static char searchBuf[128] = "";
        ImGui::InputTextWithHint("##search", "Search...", searchBuf, sizeof(searchBuf));

        ImGui::SetCursorScreenPos(ImVec2(start_pos.x, end_pos.y));

        ImGui::PopStyleVar();
        ImGui::PopItemWidth();

        ImGui::SetWindowFontScale(1.f);

        ImGui::Dummy(ImVec2(0.f, 0.f));
    }

    bool DrawModCard(const ImGuiWindow* window, ImDrawList* draw_list, const char* id, const char* name, Module* mod) {

    }
}

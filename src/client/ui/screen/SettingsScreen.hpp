#pragma once
/*#include <algorithm>
#include "IScreen.hpp"
#include <imgui.h>
#include <imgui_internal.h>
#include <core/Logger.hpp>
#include <client/modules/Module.hpp>
#include <client/Client.hpp>

class SettingsScreen : public IScreen {
public:
    SettingsScreen() : IScreen("SettingsScreen") {}

    void DrawModuleItem(Module* mod) {
        ImVec2 cardSize = ImVec2(180.f, 180.f);
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        // --- Geometry Setup ---
        ImVec2 p_min = ImGui::GetCursorScreenPos();
        ImVec2 p_max = ImVec2(p_min.x + cardSize.x, p_min.y + cardSize.y);
        ImRect totalRect(p_min, p_max);

        float bottomRowHeight = 40.0f;
        float topAreaHeight = cardSize.y - bottomRowHeight;
        float cardRounding = 12.0f;

        // 1. Top Area (Content)
        ImRect topRect(p_min, ImVec2(p_max.x, p_min.y + topAreaHeight));

        // 2. Bottom Area Split
        // We make the settings button a square (width = height)
        float settingsBtnWidth = bottomRowHeight;
        float enableBtnWidth = cardSize.x - settingsBtnWidth;

        // Left side: Enable Button
        ImRect enableRect(
            ImVec2(p_min.x, p_max.y - bottomRowHeight),
            ImVec2(p_min.x + enableBtnWidth, p_max.y)
        );

        // Right side: Settings Button
        ImRect settingsRect(
            ImVec2(p_min.x + enableBtnWidth, p_max.y - bottomRowHeight),
            p_max
        );

        // --- IDs for interactions ---
        ImGuiID topID = window->GetID((mod->getName() + "_top").c_str());
        ImGuiID enableID = window->GetID((mod->getName() + "_enable").c_str());
        ImGuiID settingsID = window->GetID((mod->getName() + "_settings").c_str());

        // ========================================================
        // Interaction Logic (Invisible Buttons)
        // ========================================================

        // 1. Top Area (Optional: Dragging or Selecting?)
        // Leaving this empty or generic since settings moved to bottom
        ImGui::SetCursorScreenPos(topRect.Min);
        if (ImGui::InvisibleButton(("##" + mod->getName() + "_top_btn").c_str(), topRect.GetSize())) {
            // Optional: Select module logic
        }
        bool topHovered = ImGui::IsItemHovered();

        // 2. Enable Button Interaction (Left)
        ImGui::SetCursorScreenPos(enableRect.Min);
        if (ImGui::InvisibleButton(("##" + mod->getName() + "_enable_btn").c_str(), enableRect.GetSize())) {
            mod->enabled = !mod->enabled;
        }
        bool enableHovered = ImGui::IsItemHovered();
        bool enableActive = ImGui::IsItemActive();

        // 3. Settings Button Interaction (Right)
        ImGui::SetCursorScreenPos(settingsRect.Min);
        if (ImGui::InvisibleButton(("##" + mod->getName() + "_settings_btn").c_str(), settingsRect.GetSize())) {
            // Logic to open settings screen moved here
            $log_info("Opened settings for: {}", mod->getName());
        }
        bool settingsHovered = ImGui::IsItemHovered();
        bool settingsActive = ImGui::IsItemActive();


        // ========================================================
        // Rendering Logic (Manual Drawing)
        // ========================================================

        // --- 1. Draw Main Card Background & Border ---
        ImU32 cardBgCol = IM_COL32(30, 30, 35, 255);
        ImU32 borderColor = IM_COL32(60, 60, 65, 255);

        draw_list->AddRectFilled(totalRect.Min, totalRect.Max, cardBgCol, cardRounding);

        if (topHovered) {
            draw_list->AddRectFilled(topRect.Min, topRect.Max, IM_COL32(255, 255, 255, 15), cardRounding, ImDrawFlags_RoundCornersTop);
        }

        // --- 2. Draw Top Content (Icon & Text) ---
        ImVec2 topCenter = topRect.GetCenter();
        ImVec2 textSize = ImGui::CalcTextSize(mod->getName().c_str());
        ImVec2 textPos = ImVec2(topCenter.x - textSize.x * 0.5f, topCenter.y + 15.0f);
        draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), mod->getName().c_str());

        // --- 3. Draw Separator Line (Horizontal) ---
        draw_list->AddLine(ImVec2(p_min.x, enableRect.Min.y), ImVec2(p_max.x, enableRect.Min.y), IM_COL32(50, 50, 55, 255), 1.0f);

        // --- 4. Draw Enable Button (Left) ---
        ImU32 enableBgCol;
        ImU32 enableTextCol = IM_COL32(255, 255, 255, 255);
        const char* statusText;

        if (mod->enabled) {
            enableBgCol = enableActive ? IM_COL32(35, 130, 75, 255) : IM_COL32(46, 174, 98, 255);
            statusText = "ENABLED";
        }
        else {
            enableBgCol = enableActive ? IM_COL32(50, 50, 50, 255) : IM_COL32(70, 70, 70, 255);
            statusText = "DISABLED";
            enableTextCol = IM_COL32(180, 180, 180, 255);
        }

        // ROUNDING: Only Bottom Left corner
        draw_list->AddRectFilled(enableRect.Min, enableRect.Max, enableBgCol, cardRounding, ImDrawFlags_RoundCornersBottomLeft);

        ImVec2 statusTextSize = ImGui::CalcTextSize(statusText);
        ImVec2 enableCenter = enableRect.GetCenter();
        draw_list->AddText(ImVec2(enableCenter.x - statusTextSize.x * 0.5f, enableCenter.y - statusTextSize.y * 0.5f), enableTextCol, statusText);

        // --- 5. Draw Settings Button (Right) ---
        // Slightly lighter gray than card bg, or highlighted if hovered
        ImU32 settingsBgCol = settingsActive ? IM_COL32(70, 70, 75, 255) : (settingsHovered ? IM_COL32(60, 60, 65, 255) : IM_COL32(45, 45, 50, 255));

        // ROUNDING: Only Bottom Right corner
        draw_list->AddRectFilled(settingsRect.Min, settingsRect.Max, settingsBgCol, cardRounding, ImDrawFlags_RoundCornersBottomRight);

        // Draw Gear/Settings Text or Icon
        const char* settingsIcon = "..."; // Use or specific icon font if available
        ImVec2 iconSize = ImGui::CalcTextSize(settingsIcon);
        ImVec2 settingsCenter = settingsRect.GetCenter();
        draw_list->AddText(ImVec2(settingsCenter.x - iconSize.x * 0.5f, settingsCenter.y - iconSize.y * 0.5f), IM_COL32(200, 200, 200, 255), settingsIcon);

        // --- 6. Draw Vertical Separator Line ---
        // Optional: Draw a line between Enable and Settings buttons
        draw_list->AddLine(settingsRect.Min, ImVec2(settingsRect.Min.x, settingsRect.Max.y), IM_COL32(30, 30, 35, 100), 1.0f);

        // --- 7. Draw Final Outer Border ---
        draw_list->AddRect(totalRect.Min, totalRect.Max, borderColor, cardRounding, 0, 1.5f);

        // Advance cursor
        ImGui::SetCursorScreenPos(p_max);
        ImGui::Dummy(ImVec2(0, 10));
    }

    virtual void render() override;
};*/
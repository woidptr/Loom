#pragma once
#include <algorithm>
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

        float bottomButtonHeight = 40.0f; // Height of the green "ENABLED" area
        float topAreaHeight = cardSize.y - bottomButtonHeight;
        float cardRounding = 12.0f;

        // Define the split rectangles
        ImRect topRect(p_min, ImVec2(p_max.x, p_min.y + topAreaHeight));
        ImRect bottomRect(ImVec2(p_min.x, p_max.y - bottomButtonHeight), p_max);

        // --- IDs for interactions ---
        ImGuiID topID = window->GetID((mod->getName() + "_top").c_str());
        ImGuiID bottomID = window->GetID((mod->getName() + "_bottom").c_str());

        // ========================================================
        // Interaction Logic (Invisible Buttons)
        // ========================================================

        // 1. Top Area Interaction (Click to go to options)
        ImGui::SetCursorScreenPos(topRect.Min);
        if (ImGui::InvisibleButton(("##" + mod->getName() + "_top_btn").c_str(), topRect.GetSize())) {
            // Logic to open settings screen goes here
            $log_info("Opened settings for: ", mod->getName());
        }
        bool topHovered = ImGui::IsItemHovered();
        bool topActive = ImGui::IsItemActive();

        // 2. Bottom Area Interaction (Click to toggle enable)
        ImGui::SetCursorScreenPos(bottomRect.Min);
        if (ImGui::InvisibleButton(("##" + mod->getName() + "_bottom_btn").c_str(), bottomRect.GetSize())) {
            mod->enabled = !mod->enabled;
        }
        bool bottomHovered = ImGui::IsItemHovered();
        bool bottomActive = ImGui::IsItemActive();


        // ========================================================
        // Rendering Logic (Manual Drawing)
        // ========================================================

        // --- 1. Draw Main Card Background & Border ---
        ImU32 cardBgCol = IM_COL32(30, 30, 35, 255); // Dark background
        ImU32 borderColor = IM_COL32(60, 60, 65, 255); // Subtle lighter border

        // Draw overall background with rounding on all corners
        draw_list->AddRectFilled(totalRect.Min, totalRect.Max, cardBgCol, cardRounding);

        // Optional: Visual feedback when hovering top area
        if (topHovered) {
            draw_list->AddRectFilled(topRect.Min, topRect.Max, IM_COL32(255, 255, 255, 15), cardRounding, ImDrawFlags_RoundCornersTop);
        }

        // --- 2. Draw Top Content (Icon & Text) ---
        // Center points for top area
        ImVec2 topCenter = topRect.GetCenter();

        // B. Draw Module Name
        ImVec2 textSize = ImGui::CalcTextSize(mod->getName().c_str());
        ImVec2 textPos = ImVec2(topCenter.x - textSize.x * 0.5f, topCenter.y + 15.0f); // Shift down below icon
        draw_list->AddText(textPos, IM_COL32(255, 255, 255, 255), mod->getName().c_str());

        // --- 3. Draw Separator Line ---
        draw_list->AddLine(bottomRect.Min, ImVec2(bottomRect.Max.x, bottomRect.Min.y), IM_COL32(50, 50, 55, 255), 1.0f);

        // --- 4. Draw Bottom Button Area ---
        ImU32 bottomBgCol;
        ImU32 bottomTextCol = IM_COL32(255, 255, 255, 255);
        const char* statusText;

        if (mod->enabled) {
            // Green color when enabled
            bottomBgCol = bottomActive ? IM_COL32(35, 130, 75, 255) : IM_COL32(46, 174, 98, 255);
            statusText = "ENABLED";
        }
        else {
            // Dark gray/red color when disabled
            bottomBgCol = bottomActive ? IM_COL32(50, 50, 50, 255) : IM_COL32(70, 70, 70, 255);
            statusText = "DISABLED";
            bottomTextCol = IM_COL32(180, 180, 180, 255);
        }

        // IMPORTANT: Draw rect with rounding ONLY on the bottom corners so it fits perfectly.
        draw_list->AddRectFilled(bottomRect.Min, bottomRect.Max, bottomBgCol, cardRounding, ImDrawFlags_RoundCornersBottom);

        // Draw status text centered in bottom area
        ImVec2 statusTextSize = ImGui::CalcTextSize(statusText);
        ImVec2 bottomCenter = bottomRect.GetCenter();
        draw_list->AddText(ImVec2(bottomCenter.x - statusTextSize.x * 0.5f, bottomCenter.y - statusTextSize.y * 0.5f), bottomTextCol, statusText);

        // --- 5. Draw Final Outer Border ---
        // drawn last to overlay everything nicely
        draw_list->AddRect(totalRect.Min, totalRect.Max, borderColor, cardRounding, 0, 1.5f);

        // Advance cursor space so the next item doesn't overlap
        ImGui::SetCursorScreenPos(p_max);
        // Add a little padding below the card
        ImGui::Dummy(ImVec2(0, 10));
    }

    void DrawModCard(Module* mod, float width, float height) {
        ImGui::PushID(mod->getName().c_str());

        // -- Style Colors for this card --
        ImVec4 bgCol = ImVec4(0.18f, 0.18f, 0.18f, 1.0f);        // Dark Card Background
        ImVec4 btnColEnv = ImVec4(0.26f, 0.59f, 0.28f, 1.0f);    // Green (Enabled)
        ImVec4 btnColDis = ImVec4(0.60f, 0.20f, 0.20f, 1.0f);    // Red (Disabled)
        ImVec4 hoverCol = ImVec4(0.22f, 0.22f, 0.22f, 1.0f);     // Card Hover

        // -- Background Group --
        ImGui::BeginGroup();

        // Draw Card Background
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetCursorScreenPos();
        drawList->AddRectFilled(p, ImVec2(p.x + width, p.y + height), ImGui::GetColorU32(bgCol), 8.0f);

        // -- 1. Icon Area (Top 60% of card) --
        // Since we don't have textures, we simulate an "Icon" with large text or a shape
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25.0f); // Padding top

        // Center the fake icon text
        float fontScale = 2.0f; // Make fake icon big
        ImGui::SetWindowFontScale(fontScale);
        float textWidth = ImGui::CalcTextSize(mod->getName().c_str()).x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (width - textWidth) * 0.5f);
        ImGui::TextColored(ImVec4(1, 1, 1, 0.9f), "%s", mod->getName().c_str());
        ImGui::SetWindowFontScale(1.0f); // Reset scale

        // Draw Real Name below icon
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10.0f);
        float nameWidth = ImGui::CalcTextSize(mod->getName().c_str()).x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (width - nameWidth) * 0.5f - (width * 0.05f)); // rough center reset
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.8f, 1.0f), "%s", mod->getName().c_str());

        // -- 2. Action Bar (Bottom of card) --
        // We manually position the cursor to the bottom area
        float bottomBarHeight = 35.0f;
        ImGui::SetCursorScreenPos(ImVec2(p.x + 5, p.y + height - bottomBarHeight - 5));

        // 'Enabled' Toggle Button (Takes up 75% width)
        float toggleWidth = (width - 15) * 0.75f;

        if (mod->enabled) {
            ImGui::PushStyleColor(ImGuiCol_Button, btnColEnv);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.65f, 0.3f, 1.0f));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, btnColDis);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.65f, 0.25f, 0.25f, 1.0f));
        }

        if (ImGui::Button(mod->enabled ? "ENABLED" : "DISABLED", ImVec2(toggleWidth, bottomBarHeight))) {
            mod->enabled = !mod->enabled;
        }
        ImGui::PopStyleColor(2);

        // Settings Button (Takes up remaining width)
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.12f, 0.12f, 1.0f)); // Darker grey for settings
        if (ImGui::Button("*", ImVec2((width - 15) * 0.25f, bottomBarHeight))) {
            // Open Settings Logic Here
        }
        ImGui::PopStyleColor();

        ImGui::EndGroup();
        ImGui::PopID();
    }

    virtual void render() override;
};
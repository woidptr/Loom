#include "HudEditorScreen.hpp"
#include "SettingsScreen.hpp"
#include <client/ui/gui/ScreenManager.hpp>
#include <imgui.h>
#include <core/Logger.hpp>
#include <events/EventHandler.hpp>
#include <events/render/HudElementRenderEvent.hpp>

void HudEditorScreen::render() {
    HudElementRenderEvent event{
        .isInEditMode = true,
    };

    EventHandler::emit(event);

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.16f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 1.f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.f));

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground;

    if (ImGui::Begin("##Settings", nullptr, flags)) {
        ImGui::SetWindowFontScale(2.0f);

        if (ImGui::Button("Settings", ImVec2(200, 70))) {
            // ScreenManager::setScreen(std::make_unique<SettingsScreen>());
        }

        ImGui::End();
    }

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(3);
}
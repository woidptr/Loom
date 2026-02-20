#include "HudScreen.hpp"
#include <events/EventHandler.hpp>
#include <events/render/HudElementRenderEvent.hpp>

void HudScreen::render() {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("##hud_overlay", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoInputs |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoSavedSettings);

    HudElementRenderEvent event{
        .isInEditMode = false,
    };

    EventHandler::emit(event);

    ImGui::End();
}
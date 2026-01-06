#include "StartScreen.hpp"
#include <imgui.h>
#include <sdk/GameContext.hpp>

void StartScreen::render() {
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.16f, 0.16f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 0.8f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.5f, 0.5f, 0.8f));

    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags flags =
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBackground;

    if (ImGui::Begin("##play_button", nullptr, flags)) {
        if (ImGui::Button("Play", ImVec2(200, 70))) {
            SceneFactory* sceneFactory = GameContext::clientInstance->getSceneFactory();
            ISceneStack* sceneStack = GameContext::clientInstance->getClientSceneStack().value;
            sceneStack->pushScreen(sceneFactory->test(), false);
        }

        ImGui::End();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
}
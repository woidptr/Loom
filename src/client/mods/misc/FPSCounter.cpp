#include "FPSCounter.hpp"
#include <format>
#include <cmath>

FPSCounter::FPSCounter() : Module("FPS Counter") {
    listeners.reserve(1);

    $add_listener(HudElementRenderEvent, &FPSCounter::onHudElementRender);
}

void FPSCounter::onHudElementRender(HudElementRenderEvent& event) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 p_min = ImVec2(10.f, 10.f);
    ImVec2 p_max = ImVec2(50.f, 50.f);

    int fps = static_cast<int>(std::round(ImGui::GetIO().Framerate));

    std::string fpsText = std::format("FPS: {}", fps);

    drawList->AddRectFilled(p_min, p_max, ImU32(1234), borderRadius);
    drawList->AddRect(p_min, p_max, ImU32(1234), borderRadius);
    // drawList->AddText(text_pos, IM_COL32(255, 255, 255, 255), fpsText.c_str());

    if (event.isInEditMode) {
        drawList->AddRectFilled(p_min, p_max, ImU32(9999), borderRadius);
    }
}
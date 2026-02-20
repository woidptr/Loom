#include "FPSCounter.hpp"
#include <format>
#include <cmath>

FPSCounter::FPSCounter() : Module("FPS Counter") {
    listeners.reserve(1);

    $add_listener(HudElementRenderEvent, &FPSCounter::onHudElementRender);
}

void FPSCounter::onHudElementRender(HudElementRenderEvent* event) {
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 p_min = ImVec2(10.f, 10.f);
    ImVec2 p_max = ImVec2(100.f, 40.f);

    int fps = static_cast<int>(std::round(ImGui::GetIO().Framerate));

    std::string fpsText = std::format("FPS: {}", fps);

    drawList->AddRectFilled(p_min, p_max, IM_COL32(0, 0, 0, 200), borderRadius);
    drawList->AddRect(p_min, p_max, IM_COL32(0, 0, 0, 255), borderRadius, ImDrawFlags_None, 2.f);

    ImVec2 boxSize = ImVec2(p_max.x - p_min.x, p_max.y - p_min.y);
    ImVec2 originalTextSize = ImGui::GetFont()->CalcTextSizeA(ImGui::GetFontSize(), FLT_MAX, 0.0f, fpsText.c_str());
    float scaleX = (boxSize.x * 0.8f) / originalTextSize.x;
    float scaleY = (boxSize.y * 0.8f) / originalTextSize.y;
    float finalScale = std::min(scaleX, scaleY);
    ImVec2 centeredPos = ImVec2(
        p_min.x + (boxSize.x - (originalTextSize.x * finalScale)) * 0.5f,
        p_min.y + (boxSize.y - (originalTextSize.y * finalScale)) * 0.5f
    );

    drawList->AddText(ImGui::GetFont(), ImGui::GetFontSize() * finalScale, p_min, IM_COL32(255, 255, 255, 255), fpsText.c_str());

    if (event->isInEditMode) {
        drawList->AddRectFilled(p_min, p_max, ImU32(9999), borderRadius);
    }
}
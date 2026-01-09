#include "FPSCounter.hpp"

#include <client/Client.hpp>

FPSCounter::FPSCounter() : Module("FPS Counter") {
    // Client::getInstance()->getUIRender()->registerImGuiDrawCallback([&]() { renderCallback(); });
}

void FPSCounter::renderCallback() {
    // fps = ImGui::GetIO().Framerate;

    // ImGui::Text("FPS: %", fps);
}
#pragma once
#include "../Module.hpp"
#include <imgui.h>
#include <events/render/HudElementRenderEvent.hpp>

class FPSCounter : public Module {
private:
    float borderRadius = 10.f;
public:
    FPSCounter();

    void onHudElementRender(HudElementRenderEvent* event);
};
#pragma once
#include <imgui.h>
#include <client/mods/Module.hpp>
#include <events/render/HudElementRenderEvent.hpp>

#include "client/Client.hpp"

class FPSCounter : public Module {
private:
    float borderRadius = 10.f;
public:
    FPSCounter();

    void onHudElementRender(HudElementRenderEvent* event);
};

$register_module(FPSCounter);

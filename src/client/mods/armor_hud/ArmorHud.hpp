#pragma once
#include <client/mods/Module.hpp>
#include <events/render/SetupAndRenderEvent.hpp>
#include <events/render/HudElementRenderEvent.hpp>

class ArmorHud : public Module {
public:
    ArmorHud();

    void onHudElementRender(HudElementRenderEvent* event);
};
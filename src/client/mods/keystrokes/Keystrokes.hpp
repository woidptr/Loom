#pragma once
#include <client/mods/Module.hpp>
#include <events/render/HudElementRenderEvent.hpp>

class Keystrokes : public Module {
public:
    Keystrokes();

    void onHudElementRender(HudElementRenderEvent* event);
};
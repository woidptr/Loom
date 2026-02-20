#pragma once
#include "../Module.hpp"
#include <events/render/SetupAndRenderEvent.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/render/HudElementRenderEvent.hpp>

class ToggleSprint : public Module {
public:
    ToggleSprint();

    void onRender(SetupAndRenderEvent* event);
    void onKey(KeyboardEvent* event);
    void onHudElementRender(HudElementRenderEvent* event);
};
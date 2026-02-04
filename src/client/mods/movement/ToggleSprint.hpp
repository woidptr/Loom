#pragma once
#include "../Module.hpp"
#include <events/render/SetupAndRenderEvent.hpp>
#include <events/input/KeyboardEvent.hpp>

class ToggleSprint : public Module {
public:
    ToggleSprint();

    void onRender(SetupAndRenderEvent& event);
    void onKey(KeyboardEvent& event);
    void renderCallback(MinecraftUIRenderContext* renderCtx);
};
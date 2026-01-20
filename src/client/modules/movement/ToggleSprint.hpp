#pragma once
#include "../Module.hpp"
#include <hooks/impl/render/SetupAndRenderHook.hpp>

class ToggleSprint : public Module {
public:
    ToggleSprint();

    void onRender(SetupAndRenderEvent& event);
    void renderCallback(MinecraftUIRenderContext* renderCtx);
};
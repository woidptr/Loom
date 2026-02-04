#pragma once
#include <events/EventHandler.hpp>

struct SetupAndRenderEvent : public CancellableEventContext {
    ScreenView* screenView;
    MinecraftUIRenderContext* renderCtx;
};
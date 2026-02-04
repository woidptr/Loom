#pragma once
#include <events/EventHandler.hpp>

struct HudElementRenderEvent : public BaseEventContext {
    bool isInEditMode;
};
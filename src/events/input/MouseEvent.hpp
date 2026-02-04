#pragma once
#include <events/EventHandler.hpp>

struct MouseEvent : public CancellableEventContext {
    char actionButtonId;
    char buttonData;
    short x;
    short y;
    short dx;
    short dy;
    bool forceMotionlessPointer;
};
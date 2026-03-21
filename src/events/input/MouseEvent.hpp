#pragma once
#include <events/EventHandler.hpp>
#include <GameInput.h>

struct MouseEvent : public CancellableEventContext {
    GameInput::v2::GameInputMouseButtons actionButtonId;
    // char buttonData;
    // short x;
    // short y;
    // short dx;
    // short dy;
    // bool forceMotionlessPointer;
};
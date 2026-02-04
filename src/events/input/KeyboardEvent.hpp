#pragma once
#include <cstdint>
#include <events/EventHandler.hpp>

struct KeyboardEvent : public CancellableEventContext {
    int16_t key;
    bool isDown;
};
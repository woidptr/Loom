#pragma once
#include <cstdint>
#include <events/EventHandler.hpp>

struct KeyboardEvent : public CancellableEventContext {
    uint32_t key;
    bool isDown;
};
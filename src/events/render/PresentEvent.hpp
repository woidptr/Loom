#pragma once
#include <events/EventHandler.hpp>
#include <dxgi1_4.h>

struct PresentEvent : public BaseEventContext {
    IDXGISwapChain3* swapChain;
};
#pragma once
#include <events/EventHandler.hpp>
#include <dxgi1_4.h>

struct ResizeBuffersEvent : public BaseEventContext {
    bool before;
    IDXGISwapChain3* swapChain;
};
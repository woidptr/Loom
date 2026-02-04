#pragma once
#include <events/EventHandler.hpp>
#include <d3d12.h>

struct ExecuteCommandListsEvent : public BaseEventContext {
    ID3D12CommandQueue* cmdQueue;
};
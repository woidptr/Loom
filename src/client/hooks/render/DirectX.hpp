#pragma once
#include "../Hook.hpp"
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <core/Logger.hpp>
#include <kiero.hpp>

struct PresentHookTag {};

class PresentHook : public Hook<PresentHookTag, HRESULT(IDXGISwapChain3*, UINT, UINT)> {
public:
    PresentHook() : Hook((uintptr_t)kiero::getMethod<&IDXGISwapChain3::Present>()) {
        this->hook();
    }
};

struct ExecuteCommandListHookTag {};

class ExecuteCommandListHook : public Hook<ExecuteCommandListHookTag, void(ID3D12CommandQueue*, UINT, ID3D12CommandList* const*)> {
public:
    ExecuteCommandListHook() : Hook((uintptr_t)kiero::getMethod<&ID3D12CommandQueue::ExecuteCommandLists>()) {
        this->hook();
    }
};

struct ResizeBuffersHookTag {};

class ResizeBuffersHook : public Hook<ResizeBuffersHookTag, HRESULT(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT)> {
public:
    ResizeBuffersHook() : Hook((uintptr_t)kiero::getMethod<&IDXGISwapChain3::ResizeBuffers>()) {
        this->hook();
    }
};
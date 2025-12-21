#pragma once
#include "../Hook.hpp"
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <core/Logger.hpp>
#include <kiero.hpp>

class PresentHook : public Hook<HRESULT(IDXGISwapChain3*, UINT, UINT)> {
public:
    PresentHook() : Hook((uintptr_t)kiero::getMethod<&IDXGISwapChain3::Present>()) {
        this->hook();
    }
};

class ExecuteCommandListHook : public Hook<void(ID3D12CommandQueue*, UINT, ID3D12CommandList* const*)> {
public:
    ExecuteCommandListHook() : Hook((uintptr_t)kiero::getMethod<&ID3D12CommandQueue::ExecuteCommandLists>()) {
        this->hook();
    }
};

class ResizeBuffersHook : public Hook<HRESULT(IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT)> {
public:
    ResizeBuffersHook() : Hook((uintptr_t)kiero::getMethod<&IDXGISwapChain3::ResizeBuffers1>()) {
        this->hook();
    }
};
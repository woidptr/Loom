#pragma once
#include "../Hook.hpp"
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <core/Logger.hpp>
#include <kiero.hpp>

class PresentHook : public Hook<HRESULT, IDXGISwapChain3*, UINT, UINT> {
public:
	static uintptr_t getAddress() {
		auto presentFunction = kiero::getMethod<&IDXGISwapChain3::Present>();

		return (uintptr_t)presentFunction;
	}

	PresentHook() : Hook(getAddress()) {
		this->hook();
	}
};

class ExecuteCommandListHook : public Hook<void, ID3D12CommandQueue*, UINT, ID3D12CommandList*> {
public:
	static uintptr_t getAddress() {
		auto executeCommandListFunction = kiero::getMethod<&ID3D12CommandQueue::ExecuteCommandLists>();

		return (uintptr_t)executeCommandListFunction;
	}

	ExecuteCommandListHook() : Hook(getAddress()) {
		this->hook();
	}
};

class ResizeBuffersHook : public Hook<HRESULT, IDXGISwapChain3*, UINT, UINT, UINT, DXGI_FORMAT, UINT> {
public:
	static uintptr_t getAddress() {
		auto resizeBuffersHookFunction = kiero::getMethod<&IDXGISwapChain3::ResizeBuffers1>();

		return (uintptr_t)resizeBuffersHookFunction;
	}

	ResizeBuffersHook() : Hook(getAddress()) {
		this->hook();
	}
};
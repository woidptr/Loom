#pragma once
#include <client/hooks/input/WndProcHook.hpp>
#include <client/hooks/input/KeyboardFeedHook.hpp>
#include <client/hooks/render/DirectX.hpp>
#include <mutex>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

class UIRender {
private:
	bool once = false;
	bool initialized = false;
	ID3D12Device* d3d12Device = nullptr;
	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandList* commandList = nullptr;
	ID3D12DescriptorHeap* srvHeap = nullptr;

	ID3D11Device* d3d11Device = nullptr;
	ID3D11DeviceContext* d3d11DeviceContext = nullptr;
	ID3D11On12Device* d3d11On12Device = nullptr;
	ID3D11Resource* backBuffer11 = nullptr;

	static inline LONG_PTR original = NULL;
public:
	UIRender(KeyboardFeedHook* keyboardFeedHook, PresentHook* presentHook, ExecuteCommandListHook* executeCommandListHook) {
		/*wndProcHook->registerCallback(
			[&](HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) -> bool {
				return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
			}
		);*/

		keyboardFeedHook->registerCallback(
			[&](uintptr_t keyCode, int state) {
				Logger::info(std::format("Keyboard pressed, state {}", state));
			}
		);

		presentHook->registerCallback(
			[&](IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
				renderCallback(swapChain, a1, a2);
			}
		);

		executeCommandListHook->registerCallback(
			[&](ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* commandList) {
				executeCommandListCallback(commandQueue, a1, commandList);
			}
		);
	}


	void initImgui(IDXGISwapChain3* swapChain);

	static LRESULT __stdcall wndProcCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

		Logger::info(std::format("Msg: {}", uMsg));

		if (uMsg == WM_POINTERUPDATE)
			Logger::info("Got WM_POINTERUPDATE");

		if (uMsg == WM_POINTERDOWN)
			Logger::info("Got WM_POINTERDOWN");

		if (uMsg == WM_POINTERUP)
			Logger::info("Got WM_POINTERUP");

		/*if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) {
			return true;
		}*/

		return CallWindowProcW((WNDPROC)original, hWnd, uMsg, wParam, lParam);
	}

	void renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2);
	void executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* commandList);
};
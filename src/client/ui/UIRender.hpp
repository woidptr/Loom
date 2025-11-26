#pragma once
#include <client/hooks/input/KeyboardFeedHook.hpp>
#include <client/hooks/input/MouseFeedHook.hpp>
#include <client/hooks/input/WindowProcHook.hpp>
#include <client/hooks/render/DirectX.hpp>
#include <mutex>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	bool draw_main_ui_flag = false;
public:
	UIRender(KeyboardFeedHook* keyboardFeedHook, MouseFeedHook* mouseFeedHook, WindowProcHook* windowProcHook, PresentHook* presentHook, ExecuteCommandListHook* executeCommandListHook);

	void initImgui(IDXGISwapChain3* swapChain);

	void keyboardCallback(int16_t key, bool isDown);

	void drawMainUI();
	void renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2);
	void executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* commandList);
};
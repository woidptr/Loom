#pragma once
#include <client/hooks/input/WindowProcHook.hpp>
#include <client/hooks/render/SetupAndRenderHook.hpp>
#include <client/hooks/render/DirectX.hpp>
#include "elements/ToastNotification.hpp"
#include "screen/ScreenManager.hpp"
#include "screen/CustomizationScreen.hpp"
#include <core/Asset.hpp>
#include <mutex>
#include <d3d12.h>
#include <d3d11.h>
#include <d3d11on12.h>
#include <winrt/base.h>
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "utils/ImGuiUtils.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

$load_asset(fonts_Arimo_Medium_ttf);

enum class RenderType {
    D3D12,
    D3D11,
};

namespace Win32Context {
    inline HWND window;
}

namespace DX12Context {
    inline winrt::com_ptr<ID3D12Device> device = nullptr;
    inline winrt::com_ptr<ID3D12DescriptorHeap> srvHeap = nullptr;
    inline ID3D12CommandQueue* cmdQueue = nullptr;
    inline ID3D12CommandList* cmdList = nullptr;
};

namespace DX11Context {
    inline winrt::com_ptr<ID3D11Device> device = nullptr;
    inline winrt::com_ptr<ID3D11DeviceContext> context = nullptr;
}

class UIRender {
private:
    bool once = false;
    bool initialized = false;

    IImguiRenderer* imguiRenderer = nullptr;
public:
    UIRender();

    void keyboardCallback(int16_t key, bool isDown);

    void initRenderer(RenderType type);

    void setupAndRenderCallback(CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx);
    void renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2);
    void executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList);
    void resizeBuffersHandler();
};
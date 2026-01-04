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

#include "utils/ImGuiUtils.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

$load_asset(fonts_Arimo_Medium_ttf);

class UIRender {
private:
    ID3D12CommandQueue* cmdQueue = nullptr;
    std::unique_ptr<IImguiRenderer> renderer = nullptr;
public:
    UIRender();

    void loadFonts();

    void keyboardCallback(int16_t key, bool isDown);
    void setupAndRenderCallback(CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx);
    void renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2);
    void executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList);
    void resizeBuffersHandler();
};
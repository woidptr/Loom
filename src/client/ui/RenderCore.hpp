#pragma once
/*#include "elements/ToastNotification.hpp"
#include "screen/ScreenManager.hpp"
#include "screen/CustomizationScreen.hpp"
#include "screen/StartScreen.hpp"
#include <core/Asset.hpp>
#include <mutex>
#include <imgui.h>

#include "utils/ImGuiUtils.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

$load_asset(fonts_Arimo_Medium_ttf);
$load_asset(fonts_Montserrat_Medium_ttf);
$load_asset(fonts_Montserrat_SemiBold_ttf);

struct ImGuiFonts {
    static inline ImFont* Montserrat = nullptr;
};

class RenderCore {
private:
    using ImGuiDrawCallback = std::function<void()>;
private:
    ID3D12CommandQueue* cmdQueue = nullptr;
    std::unique_ptr<IImguiRenderer> renderer = nullptr;
    std::vector<ImGuiDrawCallback> imguiDrawCallbacks;
public:
    RenderCore();

    void loadFonts();

    void registerImGuiDrawCallback(ImGuiDrawCallback&& fn);

    void keyboardCallback(int16_t key, bool isDown);
    void setupAndRenderCallback(CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx);
    void renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2);
    void executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList);
    void resizeBuffersHandler();
};*/
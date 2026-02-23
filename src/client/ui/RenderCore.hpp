#pragma once
#include "elements/ToastNotification.hpp"
#include "gui/ScreenManager.hpp"
#include "gui/screen/StartScreen.hpp"
#include <events/EventHandler.hpp>
#include <events/render/PresentEvent.hpp>
#include <events/render/ResizeBuffersEvent.hpp>
#include <events/render/ExecuteCommandListsEvent.hpp>
#include <events/input/WindowProcessEvent.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/input/MouseEvent.hpp>
#include <core/Asset.hpp>
#include <mutex>
#include <imgui.h>

#include "utils/ImGuiUtils.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

struct ImGuiFonts {
    static inline ImFont* Montserrat = nullptr;
};

class RenderCore {
private:
    ID3D12CommandQueue* cmdQueue = nullptr;
    std::unique_ptr<IImguiRenderer> renderer = nullptr;
    std::vector<entt::scoped_connection> listeners;
public:
    RenderCore();

    void loadFonts();

    // void keyboardCallback(int16_t key, bool isDown);
    // void setupAndRenderCallback(ScreenView* screenView, MinecraftUIRenderContext* renderCtx);
    void onKey(KeyboardEvent* event);
    void onMouse(MouseEvent* event);
    void onWindowProcess(WindowProcessEvent* event);
    void onPresentFrame(PresentEvent*);
    void onExecuteCommandLists(ExecuteCommandListsEvent* event);
    void onResizeBuffers(ResizeBuffersEvent* event);
};
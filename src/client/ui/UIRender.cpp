#include "UIRender.hpp"

#include <client/Client.hpp>
#include <imgui_internal.h>
#include <sdk/GameContext.hpp>
#include <sdk/mc/entity/components/MoveInputComponent.hpp>
#include <sdk/mc/client/gui/screens/SceneFactory.hpp>

UIRender::UIRender() {
    Hooks::windowProcHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> std::optional<LRESULT> {
            ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);

            switch (msg) {
            case WM_KEYDOWN:
                keyboardCallback((int16_t)wParam, true);

                break;
            case WM_KEYUP:
                keyboardCallback((int16_t)wParam, false);

                break;
            }

            if ((msg >= WM_KEYDOWN && msg <= WM_KEYLAST) && (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST)) {
                if (ScreenManager::getCurrentScreen()) {
                    return 1;
                    // cbCtx.cancel();
                }
            }

            return std::nullopt;
            // Logger::info(std::format("Window proc message: {}", msg));
        }
    );

    Hooks::presentHook->registerCallbackBeforeOriginal(
        [&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
            renderCallback(swapChain, a1, a2);
        }
    );

    Hooks::executeCommandListHook->registerCallbackBeforeOriginal(
        [&](CallbackContext& cbCtx, ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList) {
            executeCommandListCallback(commandQueue, a1, commandList);
        }
    );

    resizeBuffersHandler();

    Hooks::setupAndRenderHook->registerCallbackBeforeOriginal(
        [&](CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx) {
            setupAndRenderCallback(cbCtx, screenView, renderCtx);
        }
    );
}

void UIRender::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    const Asset arimoFont = $get_asset(fonts_Arimo_Medium_ttf);

    io.Fonts->AddFontFromMemoryTTF(
        (void*)arimoFont.begin(),
        arimoFont.size(),
        16.0f
    );
}

void UIRender::keyboardCallback(int16_t key, bool isDown) {
    if (key == 'L' && !isDown) {
        if (!ScreenManager::getCurrentScreen() && GameContext::clientInstance->getScreenName() == "hud_screen") {
            SceneFactory* sceneFactory = GameContext::clientInstance->getSceneFactory();
            ISceneStack* sceneStack = GameContext::clientInstance->getClientSceneStack().value;
            sceneStack->pushScreen(sceneFactory->createPauseScreen(), false);
            ScreenManager::setScreen(std::make_unique<CustomizationScreen>());
        }
    }

    if (key == VK_SHIFT && !isDown) {
        // $log_debug("Check: 0x{:X}", $get_address("SceneFactory::createPauseScreen"));
        /*auto* mic = GameContext::localPlayer->mEntityContext.tryGetComponent<MoveInputComponent>();
        mic->mInputState.mSprintDown = true;*/
        test = !test;
        ToastManager::addToast("Test", 3.0f);
        $log_debug("Current screen name: {}", GameContext::clientInstance->getScreenName());
    }

    if (key == VK_ESCAPE && !isDown) {
        ScreenManager::setScreen(nullptr);
    }
}

void UIRender::setupAndRenderCallback(CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx) {
    /*ResourceLocation rl("C:\\Users\\firel\\Downloads\\test.png", ResourceFileSystem::Raw);

    mce::TexturePtr texture = renderCtx->getTexture(rl, true);

    $log_debug("Status: {}", (bool)texture.mClientTexture.get()->mIsMissingTexture);

    renderCtx->drawImage(texture, Vec2(50, 50), Vec2(50, 50), Vec2(0, 0), Vec2(50, 50), 0);
    renderCtx->flushImages(mce::Color(1.f, 1.f, 1.f, 1.f), 1.f, HashedString::HashedString("ui_flush"));*/

    // $log_debug("ScreenContext->tessellator offset: {}", $get_offset("ScreenContext->tessellator"));

    /*if (GameContext::clientInstance == nullptr) return;

    if (GameContext::clientInstance->getScreenName() == "start_screen") {
        if (true) {
            ScreenManager::setScreen(std::make_unique<StartScreen>());
        }
    }*/

    if (ScreenManager::getCurrentScreen()) {
        cbCtx.cancel();
    }

    if (test) {
        cbCtx.cancel();
    }
}

void UIRender::renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
    if (!renderer) {
        ID3D12Device5* d3d12Device5 = nullptr;
        ID3D11Device* d3d11Device = nullptr;
        if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5)))) {
            renderer = std::make_unique<ImGuiDX12>();
            d3d12Device5->Release();
            // d3d12Device5->RemoveDevice();
        } else if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
            renderer = std::make_unique<ImGuiDX11>();
            d3d11Device->Release();
        }

        if (renderer) {
            if (!renderer->Init(swapChain, cmdQueue)) {
                $log_error("Failed to init renderer");
            }
        }
    }

    if (renderer) {
        renderer->NewFrame(swapChain);
        ImGui::NewFrame();

        for (DirectXDrawCallback& cb : drawCallbacks) {
            cb();
        }

        ToastManager::renderToasts();
        ScreenManager::render();

        ImGui::Render();
        renderer->RenderDrawData(swapChain);
    }
}

void UIRender::executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList) {
    if (!cmdQueue) {
        cmdQueue = commandQueue;
    }
}

void UIRender::resizeBuffersHandler() {
    Hooks::resizeBuffersHook->registerCallbackBeforeOriginal(
        [&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
            renderer->OnResizePre(swapChain);
        }
    );

    Hooks::resizeBuffersHook->registerCallbackAfterOriginal(
        [&](IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
            renderer->OnResizePost(swapChain);
        }
    );
}
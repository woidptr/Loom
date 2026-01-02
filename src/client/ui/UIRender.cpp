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

void UIRender::keyboardCallback(int16_t key, bool isDown) {
    if (key == 'L' && !isDown) {
        if (!ScreenManager::getCurrentScreen()) {
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
        ToastManager::addToast("Test", 3.0f);
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

    if (ScreenManager::getCurrentScreen()) {
        cbCtx.cancel();
    }
}

void UIRender::initRenderer(RenderType type) {
    if (initialized) {
        return;
    }

    switch (type) {
    case RenderType::D3D12:
        this->imguiRenderer = new ImGuiDX12();
        break;
    case RenderType::D3D11:
        this->imguiRenderer = new ImGuiDX11();
    }

    initialized = true;
}

void UIRender::renderCallback(IDXGISwapChain3* swapChain, UINT a1, UINT a2) {
    ID3D12Device5* d3d12Device5 = nullptr;
    if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5)))) {
        d3d12Device5->RemoveDevice();

        /*this->initRenderer(RenderType::D3D12);

        imguiRenderer->Init(swapChain);
        imguiRenderer->PreRender(swapChain);

        ToastManager::renderToasts();
        ScreenManager::render();

        imguiRenderer->Render(swapChain);*/
    }
    else if (SUCCEEDED(swapChain->GetDevice(IID_PPV_ARGS(DX11Context::device.put())))) {
        this->initRenderer(RenderType::D3D11);

        imguiRenderer->Init(swapChain);

        imguiRenderer->PreRender(swapChain);

        ToastManager::renderToasts();
        ScreenManager::render();

        imguiRenderer->Render(swapChain);
    }
}

void UIRender::executeCommandListCallback(ID3D12CommandQueue* commandQueue, UINT a1, ID3D12CommandList* const* commandList) {
    if (!DX12Context::cmdQueue) {
        DX12Context::cmdQueue = commandQueue;
    }

    if (!DX12Context::cmdList) {
        DX12Context::cmdList = *commandList;
    }
}

void UIRender::resizeBuffersHandler() {
    Hooks::resizeBuffersHook->registerCallbackBeforeOriginal(
        [&](CallbackContext& cbCtx, IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
            imguiRenderer->PreResize(swapChain);
        }
    );

    Hooks::resizeBuffersHook->registerCallbackAfterOriginal(
        [&](IDXGISwapChain3* swapChain, UINT a1, UINT a2, UINT a3, DXGI_FORMAT format, UINT a4) {
            imguiRenderer->PostResize(swapChain);
        }
    );
}
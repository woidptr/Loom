#include "RenderCore.hpp"

#include <client/Client.hpp>
#include <imgui_internal.h>
#include <sdk/GameContext.hpp>
#include <sdk/mc/entity/components/MoveInputComponent.hpp>
#include <sdk/mc/client/gui/screens/SceneFactory.hpp>
#include "gui/screen/HudEditorScreen.hpp"

RenderCore::RenderCore() {
    listeners.reserve(5);

    $add_listener(MouseEvent, &RenderCore::onMouse);
    $add_listener(WindowProcessEvent, &RenderCore::onWindowProcess);
    $add_listener(PresentEvent, &RenderCore::onPresentFrame);
    $add_listener(ExecuteCommandListsEvent, &RenderCore::onExecuteCommandLists);
    $add_listener(ResizeBuffersEvent, &RenderCore::onResizeBuffers);
}

void RenderCore::loadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    const Asset arimoFont = Asset("fonts/Arimo_Medium.ttf");
    const Asset montserratMedium = Asset("fonts/Arimo_SemiBold.ttf");

    // io.Fonts->AddFontFromMemoryTTF((void*)arimoFont.begin(), arimoFont.size(), 16.0f);
    ImGuiFonts::Montserrat = io.Fonts->AddFontFromMemoryTTF((void*)montserratMedium.data(), montserratMedium.size(), 16.0f);

    io.Fonts->Build();
}

void RenderCore::registerImGuiDrawCallback(ImGuiDrawCallback&& fn) {
    imguiDrawCallbacks.emplace_back(std::forward<ImGuiDrawCallback>(fn));
}

void RenderCore::onWindowProcess(WindowProcessEvent* event) {
    ImGui_ImplWin32_WndProcHandler(event->hWnd, event->msg, event->wParam, event->lParam);
}

void RenderCore::onKey(KeyboardEvent* event) {
    if (event->key == 'L' && !event->isDown) {
        if (GameContext::clientInstance->getTopScreenName() == "hud_screen" && !ScreenManager::getCurrentScreen()) {
            GameContext::sceneStack->pushScreen(GameContext::sceneFactory->createPauseScreen(), false);
            ScreenManager::setScreen(std::make_unique<HudEditorScreen>());
        }
    }
    else if (event->key == VK_ESCAPE && !event->isDown) {
        ScreenManager::setScreen(nullptr);
    }
}

//void RenderCore::keyboardCallback(int16_t key, bool isDown) {
//    if (key == 'L' && !isDown) {
//        if (!ScreenManager::getCurrentScreen() && GameContext::clientInstance->getTopScreenName() == "hud_screen") {
//            SceneFactory* sceneFactory = GameContext::clientInstance->getSceneFactory();
//            ISceneStack* sceneStack = GameContext::clientInstance->getClientSceneStack().value;
//            sceneStack->pushScreen(sceneFactory->createPauseScreen(), false);
//            ScreenManager::setScreen(std::make_unique<CustomizationScreen>());
//        }
//    }
//
//    if (key == VK_SHIFT && !isDown) {
//        // $log_debug("Check: 0x{:X}", $get_address("SceneFactory::createPauseScreen"));
//        auto* mic = GameContext::localPlayer->mEntityContext.tryGetComponent<MoveInputComponent>();
//        mic->mInputState.mSprintDown = true;
//        // ToastManager::addToast("Test", 3.0f);
//        // std::string name = GameContext::clientInstance->getTopScreenName();
//        // $log_debug("Screen name: {}", name);
//    }
//
//    if (key == VK_ESCAPE && !isDown) {
//        ScreenManager::setScreen(nullptr);
//    }
//}

//void RenderCore::setupAndRenderCallback(CallbackContext& cbCtx, ScreenView* screenView, MinecraftUIRenderContext* renderCtx) {
//    // ResourceLocation rl("textures/items/diamond_sword", ResourceFileSystem::UserPackage);
//
//    // mce::TexturePtr texture = renderCtx->getTexture(rl, false);
//
//    // $log_debug("Status: {}", (bool)texture.mClientTexture.get()->mIsMissingTexture);
//
//    // renderCtx->drawImage(texture, Vec2(50, 50), Vec2(50, 50), Vec2(0, 0), Vec2(50, 50), 0);
//    // renderCtx->flushImages(mce::Color(1.f, 1.f, 1.f, 1.f), 1.f, HashedString::HashedString("ui_texture"));
//
//    // $log_debug("ScreenContext->tessellator offset: {}", $get_offset("ScreenContext->tessellator"));
//
//    if (GameContext::clientInstance == nullptr) return;
//
//    if (GameContext::clientInstance->getScreenName() == "start_screen") {
//        if (true) {
//            ScreenManager::setScreen(std::make_unique<StartScreen>());
//        }
//    }
//
//    if (ScreenManager::getCurrentScreen()) {
//        cbCtx.cancel();
//    }
//}

void RenderCore::onMouse(MouseEvent* event) {
    /*if (!ScreenManager::getCurrentScreen()) {
        event.cancel();
    }*/
}

void RenderCore::onPresentFrame(PresentEvent* event) {
    if (!renderer) {
        ID3D12Device5* d3d12Device5 = nullptr;
        ID3D11Device* d3d11Device = nullptr;
        if (SUCCEEDED(event->swapChain->GetDevice(IID_PPV_ARGS(&d3d12Device5)))) {
            renderer = std::make_unique<ImGuiDX12>();
            d3d12Device5->Release();
            // d3d12Device5->RemoveDevice();
        } else if (SUCCEEDED(event->swapChain->GetDevice(IID_PPV_ARGS(&d3d11Device)))) {
            renderer = std::make_unique<ImGuiDX11>();
            d3d11Device->Release();
        }

        if (renderer) {
            if (!renderer->Init(event->swapChain, cmdQueue)) {
                renderer.reset();
                $log_error("Failed to init renderer");
                return;
            }
        }

        // loadFonts();
    }

    if (renderer) {
        renderer->NewFrame(event->swapChain);
        ImGui::NewFrame();

        for (ImGuiDrawCallback& cb : imguiDrawCallbacks) {
            cb();
        }

        ToastManager::renderToasts();
        ScreenManager::render();

        ImGui::Render();
        renderer->RenderDrawData(event->swapChain);
    }
}

void RenderCore::onExecuteCommandLists(ExecuteCommandListsEvent* event) {
    if (!cmdQueue) {
        cmdQueue = event->cmdQueue;
    }
}

void RenderCore::onResizeBuffers(ResizeBuffersEvent* event) {
    if (event->before) {
        renderer->OnResizePre(event->swapChain);
    } else {
        renderer->OnResizePost(event->swapChain);
    }
}
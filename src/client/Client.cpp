#include "Client.hpp"
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

void Client::construct() {
    kiero::init(kiero::RenderType::Auto);

    // hooks
    Client::windowProcHook = std::make_unique<WindowProcHook>();
    Client::getTimeOfDayHook = std::make_unique<GetTimeOfDayHook>();
    // Client::getFovHook = std::make_unique<GetFovHook>();
    // Client::levelTickHook = std::make_unique<LevelTickHook>();
    Client::getGammaHook = std::make_unique<GetGammaHook>();
    Client::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();
    Client::renderMeshHook = std::make_unique<RenderMeshHook>();
    Client::renderMeshHook->registerCallback(
        [](CallbackContext& cbCtx, ScreenContext* screenContext, Tessellator* tess, MaterialPtr* material) {
            static bool init = false;

            if (!init) {
                GameContext::materialPtr = material;

                init = true;
            }
        }
    );
    Client::presentHook = std::make_unique<PresentHook>();

    if (kiero::getRenderType() == kiero::RenderType::D3D12) {
        Client::executeCommandList = std::make_unique<ExecuteCommandListHook>();
    }

    Client::resizeBuffersHook = std::make_unique<ResizeBuffersHook>();

    // ui
    Client::uiRender = std::make_unique<UIRender>(
        Client::windowProcHook.get(),
        Client::presentHook.get(),
        Client::executeCommandList.get(),
        Client::resizeBuffersHook.get()
    );

    initModules();

    ToastManager::addToast("Client loaded", 3);
}

void Client::destruct() {
    for (Module* mod : modules) {
        delete mod;
    }

    kiero::shutdown();
}

void Client::initModules() {
    modules.push_back(new ToggleSprint(Client::setupAndRenderHook.get()));
    modules.push_back(new TimeChanger(Client::getTimeOfDayHook.get()));
    modules.push_back(new FPSCounter());
    modules.push_back(new Fullbright(Client::getGammaHook.get()));
    // modules.push_back(std::make_unique<Zoom>(Client::getFovHook.get()));
}

const std::vector<Module*> Client::getModules() {
    return modules;
}
#include "Client.hpp"
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

Client::Client() {
    kiero::init(kiero::RenderType::Auto);

    // hooks
    Hooks::windowProcHook = std::make_unique<WindowProcHook>();
    Hooks::getTimeOfDayHook = std::make_unique<GetTimeOfDayHook>();
    // Client::getFovHook = std::make_unique<GetFovHook>();
    // Client::levelTickHook = std::make_unique<LevelTickHook>();
    Hooks::getGammaHook = std::make_unique<GetGammaHook>();
    Hooks::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();
    // Client::renderMeshHook = std::make_unique<RenderMeshHook>();
    /*Client::renderMeshHook->registerCallback(
        [](CallbackContext& cbCtx, ScreenContext* screenContext, Tessellator* tess, MaterialPtr* material) {
            static bool init = false;

            if (!init) {
                GameContext::materialPtr = material;

                init = true;
            }
        }
    );*/
    Hooks::presentHook = std::make_unique<PresentHook>();

    if (kiero::getRenderType() == kiero::RenderType::D3D12) {
        Hooks::executeCommandListHook = std::make_unique<ExecuteCommandListHook>();
    }

    Hooks::resizeBuffersHook = std::make_unique<ResizeBuffersHook>();

    // ui
    uiRender = std::make_unique<UIRender>();

    initModules();

    ToastManager::addToast("Client loaded", 3);
}

Client::~Client() {
    for (Module* mod : modules) {
        delete mod;
    }

    kiero::shutdown();
}

void Client::construct() {
    if (!instance) {
        instance = new Client();
    }
}

void Client::destruct() {
    if (instance) {
        delete instance;
    }
}

Client* Client::getInstance() {
    return instance;
}

void Client::initModules() {
    modules.push_back(new ToggleSprint());
    modules.push_back(new TimeChanger());
    modules.push_back(new FPSCounter());
    modules.push_back(new Fullbright());
    // modules.push_back(new Zoom(Client::getFovHook.get()));
}

const std::vector<Module*> Client::getModules() {
    return modules;
}
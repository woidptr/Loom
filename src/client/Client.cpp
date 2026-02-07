#include "Client.hpp"
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

Client::Client() {
    // ui
    uiRender = new RenderCore();

    initModules();

    ToastManager::addToast("Client loaded", 3);
}

Client::~Client() {
    delete uiRender;

    for (Module* mod : modules) {
        delete mod;
    }
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

// RenderCore* Client::getUIRender() {
//     return uiRender;
// }

void Client::initModules() {
    modules.push_back(new ToggleSprint());
    modules.push_back(new FPSCounter());
    modules.push_back(new Replay());
}

const std::vector<Module*> Client::getModules() {
    return modules;
}
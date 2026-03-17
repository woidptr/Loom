#include "Client.hpp"
#include <core/Logger.hpp>
#include <core/FileManager.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

#include "core/settings/SettingsManager.hpp"

void Client::startup() {
    uiRender = new RenderCore();

    SettingsManager::construct();

    for (Module* mod : getModuleRegistry()) {
        modules.emplace_back(mod);
        SettingsManager::registerConfigurable(mod);
    }

    ToastManager::addToast("Client loaded", 3);
}

void Client::cleanup() {
    delete uiRender;

    for (Module* mod : modules) {
        delete mod;
    }

    SettingsManager::destruct();
}

void Client::construct() {
    if (instance == nullptr) {
        instance = new Client();
        instance->startup();
    }
}

void Client::destruct() {
    if (instance != nullptr) {
        instance->cleanup();
        delete instance;
        instance = nullptr;
    }
}

void Client::registerModule(Module* mod) {
    if (!instance) return;

    instance->modules.emplace_back(mod);
}

const std::vector<Module*> Client::getModules() {
    if (!instance) return{};

    return instance->modules;
}

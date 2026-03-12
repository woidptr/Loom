#include "Client.hpp"
#include <core/Logger.hpp>
#include <core/FileManager.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

#include "core/settings/SettingsManager.hpp"

void Client::construct() {
    uiRender = new RenderCore();

    // SettingsManager::loadProfile("Default");

    ToastManager::addToast("Client loaded", 3);
}

void Client::destruct() {
    delete uiRender;

    for (Module* mod : modules) {
        delete mod;
    }
}

void Client::registerModule(Module* mod) {
    modules.emplace_back(mod);
}

const std::vector<Module*> Client::getModules() {
    return modules;
}

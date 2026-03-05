#include "Client.hpp"
#include <core/Logger.hpp>
#include <core/FileManager.hpp>
#include <kiero.hpp>
#include <sdk/GameContext.hpp>

void Client::construct() {
    uiRender = new RenderCore();

    loadSettings();

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

void Client::loadSettings() {
    std::ifstream file(FileManager::getMainSettingsFile());

    nlohmann::json config;

    if (file.peek() != std::ifstream::traits_type::eof()) {
        $log_debug("It's fine!");
        file >> config;
    } else {
        $log_debug("Not fine!");
        config = nlohmann::json::object();
    }

    for (Module* mod : modules) {
        std::string mod_name = mod->getId();

        if (config.contains(mod_name)) {
            mod->loadSettings(config[mod_name]);
        }
    }
}

void Client::saveSettings() {
    nlohmann::json config;

    for (Module* mod : modules) {
        config[mod->getId()] = mod->saveSettings();
    }

    std::ofstream file(FileManager::getMainSettingsFile());

    file << config.dump(4);
}

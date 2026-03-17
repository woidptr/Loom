#include "SettingsManager.hpp"

#include "client/Client.hpp"
#include "client/mods/Module.hpp"

void SettingsManager::construct() {
    if (instance == nullptr) {
        instance = new SettingsManager();
    }
}

void SettingsManager::destruct() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

SettingsManager &SettingsManager::get() {
    return *instance;
}

void SettingsManager::registerConfigurable(Configurable *configurable) {
    if (!instance) return;

    instance->configurables.emplace_back(configurable);
}

void SettingsManager::load() {
    if (!instance) return;

    std::ifstream file(FileManager::getMainSettingsFile());

    nlohmann::json config;

    if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
        config = nlohmann::json::object();
    } else {
        file >> config;
    }

    for (Configurable* conf : instance->configurables) {
        std::string config_id = std::string(conf->getId());
        if (config.contains(config_id)) {
            conf->loadSettings(config[config_id]);
        }
    }
}

void SettingsManager::save() {
    if (!instance) return;

    nlohmann::json config;

    for (Configurable* conf : instance->configurables) {
        config[std::string(conf->getId())] = conf->saveSettings();
    }

    std::ofstream file(FileManager::getMainSettingsFile());
    file << config.dump(4);
}

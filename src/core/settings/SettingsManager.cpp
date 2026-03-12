#include "SettingsManager.hpp"

#include "client/Client.hpp"
#include "client/mods/Module.hpp"

void SettingsManager::registerConfigurable(Configurable *configurable) {
    configurables.emplace_back(configurable);
}

void SettingsManager::loadProfile(const std::string &profile) {
    std::ifstream file(FileManager::getProfileSettingFile(profile));

    nlohmann::json config;

    if (!file.is_open() || file.peek() == std::ifstream::traits_type::eof()) {
        config = nlohmann::json::object();
    } else {
        file >> config;
    }

    for (Module* mod : Client::getModules()) {
        std::string modId = mod->getId();

        if (config.contains(modId)) {
            mod->loadSettings(config[modId]);
        }
    }
}

void SettingsManager::saveProfile(const std::string& profile) {
    nlohmann::json config;

    for (Module* mod : Client::getModules()) {
        config[std::string(mod->getId())] = mod->saveSettings();
    }

    std::ofstream file(FileManager::getProfileSettingFile(profile));
    file << config.dump(4);
}

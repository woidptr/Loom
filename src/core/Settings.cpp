#include "Settings.hpp"

void SettingsManager::load() {
    fs::path settingsFolder = FileManager::getSettingsFolder();

    std::ifstream f(settingsFolder / "settings.json");

    json data = json::parse(f);

    RootConfig cfg;
    
    for (auto& [key, value] : data.items()) {
        cfg.mods[key] = value;
    }
}
#include "Settings.hpp"

Settings::Settings() {

}

void Settings::loadSettings() {
    fs::path settingsFolder = FileManager::getSettingsFolder();

    std::ifstream f(settingsFolder / "settings.json");

    json data = json::parse(f);
}
#include "Configurable.hpp"

void Configurable::registerSetting(ISetting *setting) {
    mSettings.emplace_back(setting);
}

nlohmann::json Configurable::saveSettings() const {
    nlohmann::json j;

    for (const ISetting* setting : mSettings) {
        setting->save(j);
    }

    return j;
}

void Configurable::loadSettings(const nlohmann::json &j) {
    for (ISetting* setting : mSettings) {
        setting->load(j);
    }
}

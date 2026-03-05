#include "Module.hpp"

Module::Module(std::string name, std::string id)
    : name(std::move(name)), id(std::move(id)) {}

Module::~Module() {
    // for (Setting* setting : settings) {
    //     delete setting;
    // }

    listeners.clear();
}

std::string Module::getName() const {
    return name;
}

std::string Module::getId() const {
    return id;
}


void Module::registerSetting(ISetting* setting) {
    settings.emplace_back(setting);
}

nlohmann::json Module::saveSettings() const {
    nlohmann::json j;
    for (const ISetting* setting : settings) {
        setting->save(j);
    }
}

void Module::loadSettings(const nlohmann::json& j) {
    for (ISetting* setting : settings) {
        setting->load(j);
    }
}
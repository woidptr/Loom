#include "Module.hpp"

Module::Module(std::string name) : name(name) {}

std::string Module::getName() {
    return name;
}

void Module::registerSetting(Setting* setting) {
    settings.emplace_back(setting);
}

std::vector<Setting*> Module::getSettings() {
    return settings;
}
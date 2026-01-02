#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Setting.hpp"

class Module {
private:
    std::string name;
    std::vector<Setting*> settings;
public:
    bool enabled = false;
public:
    Module(std::string name);
    virtual ~Module();

    std::string getName();

    void registerSetting(Setting* setting);
    std::vector<Setting*> getSettings();
};
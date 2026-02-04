#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Setting.hpp"
#include <events/EventHandler.hpp>
#include <sdk/GameContext.hpp>

class Module {
protected:
    std::string name;
    std::vector<Setting*> settings;
    std::vector<entt::scoped_connection> listeners;
public:
    bool enabled = false;
public:
    Module(std::string name);
    ~Module();

    std::string getName();

    void registerSetting(Setting* setting);
    std::vector<Setting*> getSettings();
};

struct HudElement {
    bool visible = true;

    virtual ~HudElement() = default;
    virtual void render() = 0;
};
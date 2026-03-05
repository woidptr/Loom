#pragma once
#include "ISetting.hpp"
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <events/EventHandler.hpp>
#include <sdk/GameContext.hpp>

class Module {
protected:
    std::string name;
    std::string id;
    std::vector<ISetting*> settings;
    std::vector<entt::scoped_connection> listeners;
public:
    bool enabled = false;
public:
    Module(std::string name, std::string id);
    ~Module();

    std::string getName() const;
    std::string getId() const;

    void registerSetting(ISetting* setting);
    nlohmann::json saveSettings() const;
    void loadSettings(const nlohmann::json& j);
};
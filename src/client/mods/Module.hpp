#pragma once
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <events/EventHandler.hpp>
#include <sdk/GameContext.hpp>
#include <core/settings/Configurable.hpp>

#include "core/settings/Setting.hpp"

class Module : public Configurable {
protected:
    std::string name;
    std::string id;
    std::vector<entt::scoped_connection> listeners;
public:
    Module(std::string name, std::string id);
    ~Module();

    std::string getName() const;
    std::string getId() const;

    virtual bool isEnabled() const = 0;
    virtual void toggle() = 0;
};

#pragma once
#include <string>
#include <vector>
#include <memory>
#include <json.hpp>
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
    virtual std::string getId() const override;

    virtual bool isEnabled() const = 0;
    virtual void toggle() = 0;
};

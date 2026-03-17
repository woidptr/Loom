#pragma once
#include <vector>
#include <json.hpp>
#include <core/settings/ISetting.hpp>

class Configurable {
protected:
    std::vector<ISetting*> mSettings;
public:
    virtual ~Configurable() = default;

    virtual std::string getId() const = 0;

    void registerSetting(ISetting* setting);
    nlohmann::json saveSettings() const;
    void loadSettings(const nlohmann::json& j);
};
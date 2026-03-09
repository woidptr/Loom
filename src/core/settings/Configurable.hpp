#pragma once
#include <vector>
#include <nlohmann/json.hpp>
#include <core/settings/ISetting.hpp>

class Configurable {
protected:
    std::vector<ISetting*> mSettings;
public:
    virtual ~Configurable() = default;

    void registerSetting(ISetting* setting);
    nlohmann::json saveSettings() const;
    void loadSettings(const nlohmann::json& j);
};
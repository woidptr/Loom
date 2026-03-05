#pragma once
#include <nlohmann/json.hpp>

class ISetting {
public:
    virtual ~ISetting() = default;
    virtual const char* getName() const = 0;
    virtual void save(nlohmann::json& outJson) const = 0;
    virtual void load(const nlohmann::json& inJson) = 0;
};
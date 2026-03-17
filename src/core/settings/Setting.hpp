#pragma once
#include <string>
#include <core/settings/Configurable.hpp>
#include "ISetting.hpp"
#include <json.hpp>
#include <libhat/fixed_string.hpp>
#include <imgui.h>

template <hat::fixed_string Name, typename T, hat::fixed_string Group = "">
class Setting : public ISetting {
private:
    T value;

    std::function<void(const T&)> onChangeCallback;
public:
    Setting(Configurable* parent, T default_value = T{})
        : value(default_value) {
        parent->registerSetting(this);
    }

    void setCallback(std::function<void(const T&)> callback) {
        onChangeCallback = callback;

        onChangeCallback(value);
    }

    operator T&() { return value; }
    operator const T&() const { return value; }
    Setting& operator=(const T& val) {
        if (value != val) {
            value = val;

            if (onChangeCallback) {
                onChangeCallback(value);
            }
        }

        return *this;
    }

    std::string getName() const override { return Name.str(); }
    std::string getGroup() const override { return Group.str(); }

    void save(nlohmann::json& outJson) const override {
        outJson[Name.c_str()] = value;
    }

    void load(const nlohmann::json& inJson) override {
        if (inJson.contains(Name.c_str())) {
            value = inJson[Name.c_str()].template get<T>();
        }
    }
};

template <hat::fixed_string Name, hat::fixed_string Group = "">
using BoolSetting = Setting<Name, bool, Group>;

template <hat::fixed_string Name, hat::fixed_string Group = "">
using IntSetting = Setting<Name, int, Group>;

template <hat::fixed_string Name, hat::fixed_string Group = "">
using FloatSetting = Setting<Name, float, Group>;

template <hat::fixed_string Name, hat::fixed_string Group = "">
using StringSetting = Setting<Name, std::string, Group>;

template <hat::fixed_string Name, hat::fixed_string Group = "">
using Vec2Setting = Setting<Name, ImVec2, Group>;

template <hat::fixed_string Name, hat::fixed_string Group = "">
using ColorSetting = Setting<Name, ImColor, Group>;

inline void to_json(nlohmann::json& j, const ImVec2& v) {
    j = json::array({v.x, v.y});
}

inline void from_json(const nlohmann::json& j, ImVec2& v) {
    v.x = j.at(0).get<float>();
    v.y = j.at(1).get<float>();
}

inline void to_json(nlohmann::json& j, const ImColor& c) {
    ImU32 packed = c;
    j = packed;
}

inline void from_json(const nlohmann::json& j, ImColor& c) {
    ImU32 packed = j.get<ImU32>();
    c = ImColor(packed);
}
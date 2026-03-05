#pragma once
#include <string>
#include "Module.hpp"
#include "ISetting.hpp"
#include <nlohmann/json.hpp>
#include <libhat/fixed_string.hpp>

template <hat::fixed_string Name, typename T>
class Setting : public ISetting {
private:
    T value;
public:
    Setting(Module* parent, T default_value = T{}) : value(default_value) {
        parent->registerSetting(this);
    }

    operator T&() { return value; }
    operator const T&() const { return value; }
    Setting& operator=(const T& val) { value = val; return *this; }

    const char* getName() const override { return Name.c_str(); }

    void save(nlohmann::json& outJson) const override {
        outJson[Name.c_str()] = value;
    }

    void load(const nlohmann::json& inJson) override {
        if (inJson.contains(Name.c_str())) {
            value = inJson[Name.c_str()].template get<T>();
        }
    }
};

template <hat::fixed_string Name> using BoolSetting = Setting<Name, bool>;
template <hat::fixed_string Name> using IntSetting = Setting<Name, int>;
template <hat::fixed_string Name> using FloatSetting = Setting<Name, float>;
template <hat::fixed_string Name> using StringSetting = Setting<Name, std::string>;

//class Setting {
//public:
//    std::string name;
//
//    Setting(std::string name) : name(name) {}
//
//    virtual ~Setting() = default;
//};
//
//class BoolSetting : public Setting {
//public:
//    bool value;
//
//    BoolSetting(std::string name, bool value) : Setting(name), value(value) {}
//};
//
//class FloatSetting : public Setting {
//public:
//    float value;
//
//    float minValue;
//    float maxValue;
//
//    FloatSetting(std::string name, float value, float minValue, float maxValue)
//        : Setting(std::move(name)), value(value), minValue(minValue), maxValue(maxValue) {
//    }
//};
//
//class StringSetting : public Setting {
//public:
//    std::string value;
//
//    StringSetting(std::string name, std::string value) : Setting(std::move(name)), value(value) {}
//};
//
//class KeybindSetting : public Setting {
//    int value;
//
//    KeybindSetting(std::string name, int value) : Setting(std::move(name)), value(value) {}
//};
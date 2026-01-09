#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ISetting {
protected:
    std::string name;
public:
    virtual ~ISetting() = default;

    virtual void load(const json& j) = 0;
    virtual void save(json& j) const = 0;
    virtual void draw() = 0;
public:
    std::string getName() { return name; }
};

template<typename T>
class Setting;

class SettingsGroup {
protected:
    std::vector<ISetting*> settings;
public:
    template<typename T>
    class Setting<T>& Create(const std::string& name, T value);

    void RenderAll() {
        for (ISetting* setting : settings) {
            setting->draw();
        }
    }

    void FromJson(const json& j) {
        for (ISetting* setting : settings) {
            if (j.contains(setting->getName())) {
                setting->load(j[setting->getName()]);
            }
        }
    }

    json ToJson() const {
        json j;

        for (ISetting* setting : settings) {
            setting->save(j[setting->getName()]);
        }

        return j;
    }
};

struct ModuleSetting {
    bool enabled;
    json settings;

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ModuleSetting, enabled, settings);
};

struct RootSettings {
    std::unordered_map<std::string, ModuleSetting> modules;
};

class SettingsManager {
private:
    void* config;
public:
    SettingsManager() {}

    void get() {}
    void set() {}
    void save() {}
};

class Settings {
public:
    Settings();
    ~Settings();

    void loadSettings();
};
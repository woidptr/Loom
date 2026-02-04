#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

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
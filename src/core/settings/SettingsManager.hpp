#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <json.hpp>
#include <core/settings/Configurable.hpp>

class SettingsManager {
private:
    static inline SettingsManager* instance;

    std::vector<Configurable*> configurables;

    SettingsManager() = default;
    ~SettingsManager() = default;

    SettingsManager(const SettingsManager&) = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
public:
    static void construct();
    static void destruct();
    static SettingsManager& get();

    static void registerConfigurable(Configurable* configurable);

    static void load();
    static void save();
};
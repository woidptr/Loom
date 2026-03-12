#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <nlohmann/json.hpp>
#include <core/settings/Configurable.hpp>

class SettingsManager {
private:
    static inline std::vector<Configurable*> configurables;
public:
    static void registerConfigurable(Configurable* configurable);

    void loadProfile(const std::string& profile);
    void saveProfile(const std::string& profile);
};
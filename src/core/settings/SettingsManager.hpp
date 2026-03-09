#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <nlohmann/json.hpp>

class SettingsManager {
public:
    void loadProfile(const std::string& profile);
    void saveProfile(const std::string& profile);
};
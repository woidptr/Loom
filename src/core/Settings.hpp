#pragma once
#include <fstream>
#include <core/FileManager.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct RootConfig {
    std::unordered_map<std::string, json> mods;
};

class SettingsManager {
private:
    static inline RootConfig rootConfig;
    static inline std::string rawJsonPath;
public:
    static inline void load();

    template <typename T>
    std::optional<T> getMod(const std::string& name) {
        auto it = rootConfig.mods.find(name);

        if (it == rootConfig.mods.end()) {
            return std::nullopt;
        }

        return it->second.get<T>();
    }
};
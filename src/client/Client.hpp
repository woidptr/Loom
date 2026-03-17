#pragma once
#include <vector>
#include <core/settings/SettingsManager.hpp>
#include "ui/RenderCore.hpp"
#include "mods/Module.hpp"

#define $get_modules() Client::getModules()

inline std::vector<Module*>& getModuleRegistry() {
    static std::vector<Module*> registry;
    return registry;
}

#define $register_module(module_class) \
    static_assert(std::is_base_of_v<Module, module_class>, "Error: " #module_class " does not inherit from Module!"); \
    static module_class g_##module_class##_instance; \
    static bool g_##module_class##_registered = []() { \
        getModuleRegistry().emplace_back(&g_##module_class##_instance); \
        return true; \
    }()

class Client {
private:
    static inline Client* instance = nullptr;

    RenderCore* uiRender = nullptr;
    std::vector<Module*> modules{};

    Client() = default;
    ~Client() = default;

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void startup();
    void cleanup();
public:
    static void construct();
    static void destruct();

    static void registerModule(Module* mod);
    static const std::vector<Module*> getModules();
};
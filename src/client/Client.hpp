#pragma once
#include <vector>
#include <core/settings/SettingsManager.hpp>
#include "ui/RenderCore.hpp"
#include "mods/Module.hpp"

#define $get_modules() Client::getModules()

#define $register_module(module_class) \
    static_assert(std::is_base_of_v<Module, module_class>, "Error: " #module_class " does not inherit from Module!"); \
    static module_class g_##module_class##_instance; \
    static bool g_##module_class##_registered = []() { \
        Client::registerModule(&g_##module_class##_instance); \
        SettingsManager::registerConfigurable(&g_##module_class##_instance); \
        return true; \
    }()

class Client {
private:
    static inline RenderCore* uiRender = nullptr;
    static inline std::vector<Module*> modules{};
public:
    static void construct();
    static void destruct();

    static void registerModule(Module* mod);
    static const std::vector<Module*> getModules();
};
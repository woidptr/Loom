#pragma once
#include <vector>

// ui
#include "ui/RenderCore.hpp"

// modules
#include "mods/Module.hpp"
#include "mods/movement/ToggleSprint.hpp"
#include "mods/misc/replay/Replay.hpp"

#define $get_modules() Client::getInstance()->getModules()

class Client {
private:
    static inline Client* instance = nullptr;
private:
    RenderCore* uiRender = nullptr;

    // modules
    std::vector<Module*> modules{};
public:
    Client();
    ~Client();

    static void construct();
    static void destruct();
    static Client* getInstance();

    // RenderCore* getUIRender();

    void initModules();
    const std::vector<Module*> getModules();
};
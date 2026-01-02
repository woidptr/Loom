#pragma once
#include <vector>

// hooks
#include "hooks/input/WindowProcHook.hpp"
#include "hooks/world/GetFovHook.hpp"
#include "hooks/world/GetGammaHook.hpp"
#include "hooks/world/GetTimeOfDayHook.hpp"
#include "hooks/world/LevelTickHook.hpp"
#include "hooks/render/SetupAndRenderHook.hpp"
#include "hooks/render/RenderMeshHook.hpp"
#include "hooks/render/DirectX.hpp"

// ui
#include "ui/UIRender.hpp"

// modules
#include "modules/Module.hpp"
#include "modules/movement/ToggleSprint.hpp"
#include "modules/world/Fullbright.hpp"
#include "modules/world/TimeChanger.hpp"
#include "modules/gui/FPSCounter.hpp"
#include "modules/gui/Zoom.hpp"

#include <client/hooks/HookRegistry.hpp>

#define $get_modules() Client::getInstance()->getModules()

class Client {
private:
    static inline Client* instance = nullptr;
private:
    // ui
    std::unique_ptr<UIRender> uiRender = nullptr;

    // modules
    std::vector<Module*> modules{};
public:
    Client();
    ~Client();

    static void construct();
    static void destruct();
    static Client* getInstance();

    void initModules();
    const std::vector<Module*> getModules();
};
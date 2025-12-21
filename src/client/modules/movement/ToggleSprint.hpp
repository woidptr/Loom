#pragma once
#include "../Module.hpp"
#include <client/hooks/world/LevelTickHook.hpp>
#include <client/hooks/render/SetupAndRenderHook.hpp>
#include <client/hooks/render/DirectX.hpp>

class ToggleSprint : public Module {
public:
    ToggleSprint(SetupAndRenderHook* setupAndRenderHook);

    void tickCallback(void* a1);
};
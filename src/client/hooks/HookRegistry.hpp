#pragma once

// input
#include <client/hooks/input/WindowProcHook.hpp>

// render
#include <client/hooks/render/DirectX.hpp>
#include <client/hooks/render/RenderMeshHook.hpp>
#include <client/hooks/render/SetupAndRenderHook.hpp>

// world
#include <client/hooks/world/GetFovHook.hpp>
#include <client/hooks/world/GetGammaHook.hpp>
#include <client/hooks/world/GetTimeOfDayHook.hpp>
#include <client/hooks/world/LevelTickHook.hpp>

namespace Hooks {
    inline std::unique_ptr<WindowProcHook> windowProcHook = nullptr;

    inline std::unique_ptr<PresentHook> presentHook = nullptr;
    inline std::unique_ptr<ExecuteCommandListHook> executeCommandListHook = nullptr;
    inline std::unique_ptr<ResizeBuffersHook> resizeBuffersHook = nullptr;
    inline std::unique_ptr<SetupAndRenderHook> setupAndRenderHook = nullptr;

    inline std::unique_ptr<GetFovHook> getFovHook = nullptr;
    inline std::unique_ptr<GetGammaHook> getGammaHook = nullptr;
    inline std::unique_ptr<GetTimeOfDayHook> getTimeOfDayHook = nullptr;
    inline std::unique_ptr<LevelTickHook> levelTickHook = nullptr;
}
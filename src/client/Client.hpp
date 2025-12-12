#pragma once
#include <vector>

// hooks
#include "hooks/Hook.hpp"
#include "hooks/input/WindowProcHook.hpp"
#include "hooks/world/GetFovHook.hpp"
#include "hooks/world/GetTimeOfDayHook.hpp"
#include "hooks/world/LevelTickHook.hpp"
#include "hooks/render/SetupAndRenderHook.hpp"
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

class Client {
private:
	// hooks
	static inline std::unique_ptr<WindowProcHook> windowProcHook = nullptr;
	static inline std::unique_ptr<GetTimeOfDayHook> getTimeOfDayHook = nullptr;
	static inline std::unique_ptr<GetFovHook> getFovHook = nullptr;
	static inline std::unique_ptr<LevelTickHook> levelTickHook = nullptr;
	static inline std::unique_ptr<SetupAndRenderHook> setupAndRenderHook = nullptr;
	static inline std::unique_ptr<PresentHook> presentHook = nullptr;
	static inline std::unique_ptr<ExecuteCommandListHook> executeCommandList = nullptr;
	static inline std::unique_ptr<ResizeBuffersHook> resizeBuffersHook = nullptr;

	// ui
	static inline std::unique_ptr<UIRender> uiRender = nullptr;

	// modules
	static inline std::vector<std::unique_ptr<Module>> modules{};
public:
	static void construct();
	static void destruct();

	static void initModules();
	static const std::vector<std::unique_ptr<Module>>& getModules();
};
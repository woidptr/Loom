#pragma once
#include <vector>

// hooks
#include "hooks/Hook.hpp"
#include "hooks/input/WndProcHook.hpp"
#include "hooks/world/LevelTickHook.hpp"
#include "hooks/render/SetupAndRenderHook.hpp"

// modules
#include "modules/Module.hpp"
#include "modules/movement/ToggleSprint.hpp"

class Client {
private:
	// hooks
	static inline std::unique_ptr<LevelTickHook> levelTickHook = nullptr;
	static inline std::unique_ptr<SetupAndRenderHook> setupAndRenderHook = nullptr;

	// modules
	static inline std::unique_ptr<ToggleSprint> toggleSprintModule = nullptr;
public:
	static void construct();
	static void destruct();
};
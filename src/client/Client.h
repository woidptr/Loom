#pragma once
#include <vector>

// hooks
#include "hooks/Hook.h"
#include "hooks/input/WndProcHook.h"
#include "hooks/world/LevelTickHook.h"
#include "hooks/render/SetupAndRenderHook.h"

// modules
#include "modules/Module.h"
#include "modules/movement/ToggleSprint.h"

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
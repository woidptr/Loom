#pragma once
#include <vector>

// hooks
#include "hooks/Hook.hpp"
#include "hooks/input/KeyboardFeedHook.hpp"
#include "hooks/input/WndProcHook.hpp"
#include "hooks/world/LevelTickHook.hpp"
#include "hooks/render/SetupAndRenderHook.hpp"
#include "hooks/render/DirectX.hpp"

// ui
#include "ui/UIRender.hpp"

// modules
#include "modules/Module.hpp"
#include "modules/movement/ToggleSprint.hpp"

class Client {
private:
	// hooks
	// static inline std::unique_ptr<WndProcHook> wndProcHook = nullptr;
	static inline std::unique_ptr<KeyboardFeedHook> keyboardFeedHook = nullptr;
	static inline std::unique_ptr<LevelTickHook> levelTickHook = nullptr;
	static inline std::unique_ptr<SetupAndRenderHook> setupAndRenderHook = nullptr;
	static inline std::unique_ptr<PresentHook> presentHook = nullptr;
	static inline std::unique_ptr<ExecuteCommandListHook> executeCommandList = nullptr;

	// ui
	static inline std::unique_ptr<UIRender> uiRender = nullptr;

	// modules
	static inline std::unique_ptr<ToggleSprint> toggleSprintModule = nullptr;
public:
	static void checkSignatures();
	static void construct();
	static void destruct();
};
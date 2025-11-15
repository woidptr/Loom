#pragma once
#include <vector>

// hooks
#include "hooks/Hook.h"
#include "hooks/input/WndProcHook.h"
#include "hooks/render/SetupAndRenderHook.h"

// modules
#include "modules/Module.h"
#include "modules/movement/ToggleSprint.h"

class Client {
private:
	static WndProcHook* wndProcHook;
	static SetupAndRenderHook* setupAndRenderHook;
public:
	static void construct();
	static void destruct();
};
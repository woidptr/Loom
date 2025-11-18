#include "Client.h"
#include <MinHook.h>
#include "../core/Logger.h"
#include <format>

#include "hooks/input/WndProcHook.h"

void Client::construct() {
	MH_Initialize();

	// hooks
	Client::levelTickHook = std::make_unique<LevelTickHook>();
	Client::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();

	// modules
	Client::toggleSprintModule = std::make_unique<ToggleSprint>(Client::levelTickHook.get(), Client::setupAndRenderHook.get());
}

void Client::destruct() {
	MH_Uninitialize();
}

#include "Client.hpp"
#include <MinHook.h>
#include <core/Logger.hpp>
#include <format>

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

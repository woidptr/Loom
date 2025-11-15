#include "Client.h"
#include <MinHook.h>
#include "../core/Logger.h"
#include <format>

#include "hooks/input/WndProcHook.h"

WndProcHook* Client::wndProcHook = nullptr;
SetupAndRenderHook* Client::setupAndRenderHook = nullptr;

void Client::construct() {
	MH_Initialize();

	Client::wndProcHook = new WndProcHook();
	Client::setupAndRenderHook = new SetupAndRenderHook();

	// ToggleSprint* toggleSprint = new ToggleSprint();

	// Client::setupAndRenderHook->registerCallback(&toggleSprint->tickCallback);

	// hooks.push_back(new SetupAndRenderHook());
	// hooks.push_back(new WndProcHook());
}

void Client::destruct() {
	MH_Uninitialize();
}

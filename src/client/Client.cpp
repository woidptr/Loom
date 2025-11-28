#include "Client.hpp"
#include <MinHook.h>
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>

void Client::construct() {
	MH_Initialize();

	// hooks
	Client::windowProcHook = std::make_unique<WindowProcHook>();
	Client::getTimeOfDayHook = std::make_unique<GetTimeOfDayHook>();
	Client::levelTickHook = std::make_unique<LevelTickHook>();
	Client::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();
	Client::presentHook = std::make_unique<PresentHook>();
	Client::executeCommandList = std::make_unique<ExecuteCommandListHook>();

	// ui
	Client::uiRender = std::make_unique<UIRender>(
		Client::windowProcHook.get(),
		Client::presentHook.get(),
		Client::executeCommandList.get()
	);

	// modules
	Client::toggleSprintModule = std::make_unique<ToggleSprint>(Client::levelTickHook.get(), Client::setupAndRenderHook.get());
	Client::timeChangerModule = std::make_unique<TimeChanger>(Client::getTimeOfDayHook.get());
}

void Client::destruct() {
	MH_Uninitialize();
}

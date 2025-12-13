#include "Client.hpp"
#include <MinHook.h>
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>
#include <kiero.hpp>

void Client::construct() {
	MH_Initialize();

	kiero::init(kiero::RenderType::Auto);

	// hooks
	Client::windowProcHook = std::make_unique<WindowProcHook>();
	Client::getTimeOfDayHook = std::make_unique<GetTimeOfDayHook>();
	// Client::getFovHook = std::make_unique<GetFovHook>();
	// Client::levelTickHook = std::make_unique<LevelTickHook>();
	Client::getGammaHook = std::make_unique<GetGammaHook>();
	Client::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();
	Client::presentHook = std::make_unique<PresentHook>();
	Client::executeCommandList = std::make_unique<ExecuteCommandListHook>();
	Client::resizeBuffersHook = std::make_unique<ResizeBuffersHook>();

	// ui
	Client::uiRender = std::make_unique<UIRender>(
		Client::windowProcHook.get(),
		Client::presentHook.get(),
		Client::executeCommandList.get(),
		Client::resizeBuffersHook.get()
	);

	initModules();
}

void Client::destruct() {
	MH_Uninitialize();

	kiero::shutdown();
}

void Client::initModules() {
	modules.push_back(std::make_unique<ToggleSprint>(Client::setupAndRenderHook.get()));
	modules.push_back(std::make_unique<TimeChanger>(Client::getTimeOfDayHook.get()));
	modules.push_back(std::make_unique<FPSCounter>());
	modules.push_back(std::make_unique<Fullbright>(Client::getGammaHook.get()));
	// modules.push_back(std::make_unique<Zoom>(Client::getFovHook.get()));
}

const std::vector<std::unique_ptr<Module>>& Client::getModules() {
	return modules;
}
#include "Client.hpp"
#include <MinHook.h>
#include <core/Logger.hpp>
#include <format>
#include <core/Signatures.hpp>

void Client::construct() {
	MH_Initialize();

	SignatureRegistry::registerSignatures();

	if (!SignatureRegistry::performHealthCheck()) {
		return;
	}

	// hooks
	Client::keyboardFeedHook = std::make_unique<KeyboardFeedHook>();
	Client::mouseFeedHook = std::make_unique<MouseFeedHook>();
	Client::windowProcHook = std::make_unique<WindowProcHook>();
	Client::levelTickHook = std::make_unique<LevelTickHook>();
	Client::setupAndRenderHook = std::make_unique<SetupAndRenderHook>();
	Client::presentHook = std::make_unique<PresentHook>();
	Client::executeCommandList = std::make_unique<ExecuteCommandListHook>();

	// ui
	Client::uiRender = std::make_unique<UIRender>(
		Client::keyboardFeedHook.get(),
		Client::mouseFeedHook.get(),
		Client::windowProcHook.get(),
		Client::presentHook.get(),
		Client::executeCommandList.get()
	);

	// modules
	Client::toggleSprintModule = std::make_unique<ToggleSprint>(Client::levelTickHook.get(), Client::setupAndRenderHook.get());
}

void Client::destruct() {
	MH_Uninitialize();
}

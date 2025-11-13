#include "Client.h"
#include <MinHook.h>
#include "hooks/render/DirectXHook.h"
#include "../core/Logger.h"
#include <format>

#include "hooks/render/SetupAndRenderHook.h"

std::vector<Hook*> Client::hooks;

void Client::construct() {
	MH_Initialize();

	hooks.push_back(new SetupAndRenderHook());
}

void Client::destruct() {
	MH_Uninitialize();
}

//void Client::test() {
//	DirectXHook dx = DirectXHook();
//	if (dx.getDirectXVersion() == DirectXVersion::D3D12) {
//		Logger::debug("DirectX version 12 is found");
//	}
//	else {
//		Logger::debug("Somegthing else if found");
//	}
//
//	Logger::debug(std::format("Present address: {}", dx.address));
//}

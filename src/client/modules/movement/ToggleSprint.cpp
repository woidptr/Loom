#include "ToggleSprint.h"
#include <core/Logger.h>
#include <sdk/SDK.h>

bool init = false;

void ToggleSprint::tickCallback(void* a1) {
	// Logger::info("Tick");
	if (SDK::clientInstance->getLocalPlayer() != nullptr) {
		// Logger::info("Found renderCtx pointer");
		/*if (SDK::renderCtx->clientInstance != nullptr) {
			Logger::info("Found clientInstance pointer");
		}*/

		/*uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);

		uintptr_t vtableAddr = ((uintptr_t)(*reinterpret_cast<void***>(SDK::renderCtx->clientInstance))) - moduleBase;

		Logger::info(std::format("ClientInstance vtable address: {}", vtableAddr));*/
		Logger::info("Test");

		/*LocalPlayer* localPlayer = SDK::renderCtx->clientInstance->getLocalPlayer();

		if (localPlayer != nullptr) {
			Logger::info("Found localPlayer pointer");
		}*/

		SDK::clientInstance->getLocalPlayer()->setSprinting(true);
	}
}

void ToggleSprint::renderCallback(void* a1, MinecraftUIRenderContext* renderCtx) {
	if (!init) {
		SDK::renderCtx = renderCtx;
		SDK::clientInstance = renderCtx->clientInstance;

		uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
		uintptr_t vtableAddr = ((uintptr_t)(*reinterpret_cast<void***>(SDK::renderCtx->clientInstance)[219])) - moduleBase;
		Logger::info(std::format("ClientInstance vtable address: 0x{:X}", vtableAddr));

		init = true;
	}
}
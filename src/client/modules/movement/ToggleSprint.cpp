#include "ToggleSprint.h"
#include <core/Logger.h>
#include <sdk/GameContext.h>
#include <core/Signatures.h>

bool init = false;

void ToggleSprint::tickCallback(void* a1) {
	// Logger::info("Tick");
	if (GameContext::clientInstance->getLocalPlayer() != nullptr) {
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

		GameContext::clientInstance->getLocalPlayer()->setSprinting(true);
	}
}

void ToggleSprint::renderCallback(void* a1, MinecraftUIRenderContext* renderCtx) {
	if (!init) {
		GameContext::registerRenderContext(renderCtx);

		uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
		uintptr_t vtableAddr = ((uintptr_t)(*reinterpret_cast<void***>(GameContext::clientInstance)[31])) - moduleBase;
		Logger::info(std::format("ClientInstance vtable address: 0x{:X}", vtableAddr));

		GameContext::renderCtx->getUIScene();
		Logger::info(std::format("UIScene offset: 0x{:X}", Signatures::MinecraftUIRenderContext::UIScene.getAddress()));
		// Logger::info(std::format("ScreenContext runtime address: 0x{:X}", GameContext::renderCtx->getScreenContext()));

		init = true;
	}
}
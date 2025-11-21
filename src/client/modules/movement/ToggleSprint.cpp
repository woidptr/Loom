#include "ToggleSprint.hpp"
#include <core/Logger.hpp>
#include <sdk/GameContext.hpp>
#include <core/Signatures.hpp>

bool init = false;

void ToggleSprint::tickCallback(void* a1) {

}

void ToggleSprint::renderCallback(void* a1, MinecraftUIRenderContext* renderCtx) {
	RectangleArea rect(50, 100, 50, 100);
	mce::Color color(1.f, 1.f, 1.f, 1.f);
	renderCtx->fillRectangle(rect, color, 1.f);

	if (!init) {
		GameContext::registerRenderContext(renderCtx);

		uintptr_t moduleBase = (uintptr_t)GetModuleHandleA(NULL);
		uintptr_t vtableAddr = ((uintptr_t)(*reinterpret_cast<void***>(GameContext::clientInstance)[31])) - moduleBase;
		Logger::info(std::format("ClientInstance vtable address: 0x{:X}", vtableAddr));

		// GameContext::renderCtx->getUIScene();
		// Logger::info(std::format("UIScene offset: 0x{:X}", Signatures::MinecraftUIRenderContext::UIScene.getAddress()));
		// Logger::info(std::format("ScreenContext runtime address: 0x{:X}", GameContext::renderCtx->getScreenContext()));

		init = true;
	}
}
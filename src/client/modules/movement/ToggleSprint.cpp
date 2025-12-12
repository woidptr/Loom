#include "ToggleSprint.hpp"
#include <core/Logger.hpp>
#include <sdk/GameContext.hpp>
#include <core/Signatures.hpp>

ToggleSprint::ToggleSprint(SetupAndRenderHook* setupAndRenderHook) : Module("Toggle Sprint") {
	setupAndRenderHook->registerCallback(
		[&](CallbackContext& cbCtx, void* screenView, MinecraftUIRenderContext* renderCtx) {
			void** vtable = *(void***)renderCtx;
			uintptr_t staticVtable = reinterpret_cast<uintptr_t>(vtable) - reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
			Logger::info(std::format("MinecraftUIRenderContext vtable address: 0x{:X}", staticVtable));
			// Logger::info(std::format("Text alpha: {}", renderCtx->getTextAlpha()));
			RectangleArea rect = RectangleArea{ 10, 300, 10, 300 };
			mce::Color color = mce::Color{ 1.f, 1.f, 1.f, 1.f };
			renderCtx->drawRectangle(rect, color, 1.f, 5);
		}
	);
}

void ToggleSprint::tickCallback(void* a1) {

}
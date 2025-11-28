#include "../Module.hpp"
#include <client/hooks/world/LevelTickHook.hpp>
#include <client/hooks/render/SetupAndRenderHook.hpp>
#include <client/hooks/render/DirectX.hpp>

class ToggleSprint : public Module {
private:
	bool imguiInitialized = false;
public:
	ToggleSprint(LevelTickHook* levelTickHook, SetupAndRenderHook* setupAndRenderHook) : Module("Toggle Sprint") {
		levelTickHook->registerCallback(
			[&](CallbackContext<void>& cbCtx, void* a1) {
				tickCallback(a1);
			}
		);

		setupAndRenderHook->registerCallback(
			[&](auto& cbCtx, void* a1, MinecraftUIRenderContext* renderCtx) {
				renderCallback(a1, renderCtx);
			}
		);
	}

	void tickCallback(void* a1);
	void renderCallback(void* a1, MinecraftUIRenderContext* renderCtx);
};
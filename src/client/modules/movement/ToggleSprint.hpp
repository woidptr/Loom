#include "../Module.hpp"
#include "../../hooks/world/LevelTickHook.hpp"
#include "../../hooks/render/SetupAndRenderHook.hpp"
#include "../../hooks/render/DirectX.hpp"

class ToggleSprint : public Module {
private:
	bool imguiInitialized = false;
public:
	ToggleSprint(LevelTickHook* levelTickHook, SetupAndRenderHook* setupAndRenderHook) : Module("Toggle Sprint") {
		levelTickHook->registerCallback(
			[&](void* a1) {
				tickCallback(a1);
			}
		);

		setupAndRenderHook->registerCallback(
			[&](void* a1, MinecraftUIRenderContext* renderCtx) {
				renderCallback(a1, renderCtx);
			}
		);
	}

	void tickCallback(void* a1);
	void renderCallback(void* a1, MinecraftUIRenderContext* renderCtx);
};
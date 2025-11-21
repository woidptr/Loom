#include "../Module.hpp"
#include "../../hooks/world/LevelTickHook.hpp"
#include "../../hooks/render/SetupAndRenderHook.hpp"

class ToggleSprint : public Module {
private:
	LevelTickHook* levelTickHook = nullptr;
	SetupAndRenderHook* setupAndRenderHook = nullptr;
public:
	ToggleSprint(LevelTickHook* levelTickHook, SetupAndRenderHook* setupAndRenderHook) : Module("Toggle Sprint") {
		this->levelTickHook = levelTickHook;
		this->setupAndRenderHook = setupAndRenderHook;

		this->levelTickHook->registerCallback([&](void* a1) {tickCallback(a1); });
		this->setupAndRenderHook->registerCallback([&](void* a1, MinecraftUIRenderContext* renderCtx) {renderCallback(a1, renderCtx); });
	}

	void tickCallback(void* a1);
	void renderCallback(void* a1, MinecraftUIRenderContext* renderCtx);
};
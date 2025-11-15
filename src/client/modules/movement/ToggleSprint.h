#include "../Module.h"

template <typename...Args>
class ToggleSprint : public Module {
public:
	ToggleSprint() : Module("Toggle Sprint") {

	}

	void tickCallback(void* screenView, MinecraftUIRenderContext* renderCtx) {}
};
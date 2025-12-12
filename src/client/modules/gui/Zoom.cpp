#include "Zoom.hpp"

Zoom::Zoom(GetFovHook* getFovHook) : Module("Zoom") {
	getFovHook->registerCallback(
		[&](CallbackContext& cbCtx, void* a1, float a2, bool a3) {
			getFovCallback(a1, a2, a3);
		}
	);
}

void Zoom::getFovCallback(void* a1, float a2, bool a3) {
	Logger::info(std::format("Fov: {}", a2));
}
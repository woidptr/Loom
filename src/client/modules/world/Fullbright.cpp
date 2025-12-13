#include "Fullbright.hpp"

Fullbright::Fullbright(GetGammaHook* getGammaHook) : Module("Fullbright") {
	getGammaHook->registerReturnCallback(
		[&](CallbackContext& cbCtx, void* Options) -> std::optional<float> {
			return std::nullopt;
			// return 10.f;
		}
	);
}
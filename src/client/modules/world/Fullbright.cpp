#include "Fullbright.hpp"

Fullbright::Fullbright(GetGammaHook* getGammaHook) : Module("Fullbright") {
    getGammaHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, void* options) -> std::optional<float> {
            return gammaReturnCallback(cbCtx, options);
        }
    );
}

std::optional<float> Fullbright::gammaReturnCallback(CallbackContext& cbCtx, void* options) {
    return std::nullopt;
}
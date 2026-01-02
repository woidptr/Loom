#include "Fullbright.hpp"
#include <client/hooks/HookRegistry.hpp>

Fullbright::Fullbright() : Module("Fullbright") {
    Hooks::getGammaHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, void* options) -> std::optional<float> {
            return gammaReturnCallback(cbCtx, options);
        }
    );
}

std::optional<float> Fullbright::gammaReturnCallback(CallbackContext& cbCtx, void* options) {
    if (!enabled) {
        return std::nullopt;
    }

    return gamma;
}
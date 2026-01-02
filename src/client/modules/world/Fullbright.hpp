#pragma once
#include "../Module.hpp"

#include <client/hooks/world/GetGammaHook.hpp>

class Fullbright : public Module {
private:
    float gamma = 20.f;
public:
    Fullbright();

    std::optional<float> gammaReturnCallback(CallbackContext& cbCtx, void* options);
};
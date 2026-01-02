#include "TimeChanger.hpp"
#include <client/hooks/HookRegistry.hpp>

TimeChanger::TimeChanger() : Module("Time Changer") {
    Hooks::getTimeOfDayHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, void* dimension, int time, float alpha) -> std::optional<float> {
            return getTimeOfDayCallback(cbCtx, dimension, time, alpha);
        }
    );
}

std::optional<float> TimeChanger::getTimeOfDayCallback(CallbackContext& cbCtx, void* dimension, int time, float alpha) {
    /*static float value = 0.f;
    value += 0.00001f;
    value = std::fmodf(value, 1.f);

    return value;*/

    return std::nullopt;
}
#include "TimeChanger.hpp"

#include <client/Client.hpp>

TimeChanger::TimeChanger(GetTimeOfDayHook* getTimeOfDayHook) : Module("Time Changer") {
    getTimeOfDayHook->registerReturnCallback(
        [&](CallbackContext& cbCtx, void* dimension, int time, float alpha) -> float {
            return getTimeOfDayCallback(cbCtx, dimension, time, alpha);
        }
    );
}

float TimeChanger::getTimeOfDayCallback(CallbackContext& cbCtx, void* dimension, int time, float alpha) {
    static float value = 0.f;
    value += 0.00001f;
    value = std::fmodf(value, 1.f);

    return value;
}
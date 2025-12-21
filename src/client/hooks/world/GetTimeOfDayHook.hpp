#pragma once
#include "../Hook.hpp"

class GetTimeOfDayHook : public Hook<float(void*, int, float)> {
public:
    GetTimeOfDayHook() : Hook($getSignature("Dimension::getTimeOfDay")) {
        this->hook();
    }
};
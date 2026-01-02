#pragma once
#include "../Hook.hpp"

struct GetTimeOfDayHookTag {};

class GetTimeOfDayHook : public Hook<GetTimeOfDayHookTag, float(void*, int, float)> {
public:
    GetTimeOfDayHook() : Hook($get_signature("Dimension::getTimeOfDay")) {
        this->hook();
    }
};
#pragma once
#include "../Hook.hpp"

struct GetTimeOfDayHookTag {};

class GetTimeOfDayHook : public InlineHook<GetTimeOfDayHookTag, float(void*, int, float)> {
public:
    GetTimeOfDayHook() : InlineHook($get_signature("Dimension::getTimeOfDay")) {
        this->hook();
    }
};
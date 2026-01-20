#pragma once
#include <sdk/Predefine.hpp>

class Dimension {
public:
    MCAPI virtual float getTimeOfDay(int time, float a) const final;
};
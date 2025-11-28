#pragma once
#include "../Module.hpp"
#include <client/hooks/world/GetTimeOfDayHook.hpp>

class TimeChanger : public Module {
public:
	TimeChanger(GetTimeOfDayHook* getTimeOfDayHook);
};
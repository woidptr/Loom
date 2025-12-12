#pragma once
#include "../Module.hpp"
#include <client/hooks/world/GetTimeOfDayHook.hpp>

class TimeChanger : public Module {
public:
	TimeChanger(GetTimeOfDayHook* getTimeOfDayHook);

	float getTimeOfDayCallback(CallbackContext& cbCtx, void* dimension, int time, float alpha);
};
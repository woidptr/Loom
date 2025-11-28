#include "TimeChanger.hpp"

TimeChanger::TimeChanger(GetTimeOfDayHook* getTimeOfDayHook) : Module("Time Changer") {
	getTimeOfDayHook->registerCallback(
		[&](CallbackContext<float>& cbCtx, void* dimension, int time, float alpha) {
			cbCtx.overrideReturn = 0.5f;
		}
	);
}
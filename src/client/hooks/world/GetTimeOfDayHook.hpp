#pragma once
#include "../Hook.hpp"

class GetTimeOfDayHook : public Hook<float, void*, int, float>{
public:
	GetTimeOfDayHook() : Hook(SignatureRegistry::getSignature("Dimension::getTimeOfDay")) {
		this->hook();
	}
};
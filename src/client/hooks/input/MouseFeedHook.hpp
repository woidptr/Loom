#pragma once
#include "../Hook.hpp"

enum ActionButton {
	MouseMove = 0,
	LeftClick = 1,
	RightClick = 2,
	MiddleClick = 3,
};

class MouseFeedHook : public Hook<void, void*, ActionButton, int8_t, int16_t, int16_t, int16_t, int16_t, bool> {
public:
	MouseFeedHook() : Hook(SignatureRegistry::getSignature("MouseDevice::feed")) {
		this->hook();
	}
};
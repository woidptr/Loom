#pragma once
#include "../Hook.h"

class GameModeTickHook : public Hook<void, void> {
public:
	GameModeTickHook() : Hook("GameModeTickHook", 2) {
		this->hook();
	}
};
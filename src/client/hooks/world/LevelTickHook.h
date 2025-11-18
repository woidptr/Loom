#pragma once
#include "../Hook.h"
#include <core/Signatures.h>

class LevelTickHook : public Hook<void, void*> {
public:
	LevelTickHook() : Hook(Signatures::Level::tick.getName(), Signatures::Level::tick.getAddress()) {
		this->hook();
	}
};
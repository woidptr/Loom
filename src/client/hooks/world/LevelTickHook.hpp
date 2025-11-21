#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

class LevelTickHook : public Hook<void, void*> {
public:
	LevelTickHook() : Hook(Signatures::Level::tick) {
		this->hook();
	}
};
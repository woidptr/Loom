#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

class LevelTickHook : public Hook<void, void*> {
public:
	LevelTickHook() : Hook(SignatureRegistry::getSignature("Level::tick")) {
		this->hook();
	}
};
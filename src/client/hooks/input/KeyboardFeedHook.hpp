#pragma once
#include "../Hook.hpp"
#include <core/Signatures.hpp>

class KeyboardFeedHook : public Hook<void, uintptr_t, int> {
public:
	KeyboardFeedHook() : Hook(SignatureRegistry::getSignature("Keyboard::feed")) {
		this->hook();
	}
};
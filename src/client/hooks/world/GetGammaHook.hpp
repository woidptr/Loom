#pragma once
#include "../Hook.hpp"

class GetGammaHook : public Hook<float, void*> {
public:
	GetGammaHook() : Hook(SignatureRegistry::getSignature("Options::getGamma")) {
		this->hook();
	}
};
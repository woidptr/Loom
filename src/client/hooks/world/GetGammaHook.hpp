#pragma once
#include "../Hook.hpp"

class GetGammaHook : public Hook<float(void*)> {
public:
    GetGammaHook() : Hook($getSignature("Options::getGamma")) {
        this->hook();
    }
};
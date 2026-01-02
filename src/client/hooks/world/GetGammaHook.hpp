#pragma once
#include "../Hook.hpp"

struct GetGammaHookTag {};

class GetGammaHook : public Hook<GetGammaHookTag, float(void*)> {
public:
    GetGammaHook() : Hook($get_signature("Options::getGamma")) {
        this->hook();
    }
};
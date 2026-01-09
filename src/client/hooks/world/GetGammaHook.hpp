#pragma once
#include "../Hook.hpp"

struct GetGammaHookTag {};

class GetGammaHook : public InlineHook<GetGammaHookTag, float(void*)> {
public:
    GetGammaHook() : InlineHook($get_signature("Options::getGamma")) {
        this->hook();
    }
};
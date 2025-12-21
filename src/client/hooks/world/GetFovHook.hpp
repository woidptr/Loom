#pragma once
#include "../Hook.hpp"

class GetFovHook : public Hook<float(void*, float, bool)> {
public:
    GetFovHook() : Hook($getSignature("LevelRendererPlayer::getFov")) {
        this->hook();
    }
};
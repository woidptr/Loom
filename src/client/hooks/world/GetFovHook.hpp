#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/render/game/LevelRendererPlayer.hpp>

struct GetFovHookTag {};

class GetFovHook : public Hook<GetFovHookTag, float(LevelRendererPlayer*, float, bool)> {
public:
    GetFovHook() : Hook($get_signature("LevelRendererPlayer::getFov")) {
        this->hook();
    }
};
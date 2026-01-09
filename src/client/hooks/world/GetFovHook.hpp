#pragma once
#include "../Hook.hpp"
#include <sdk/mc/client/render/game/LevelRendererPlayer.hpp>

struct GetFovHookTag {};

class GetFovHook : public InlineHook<GetFovHookTag, float(LevelRendererPlayer*, float, bool)> {
public:
    GetFovHook() : InlineHook($get_signature("LevelRendererPlayer::getFov")) {
        this->hook();
    }
};
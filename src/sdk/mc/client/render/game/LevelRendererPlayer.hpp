#pragma once
#include <core/Memory.hpp>
#include <sdk/mc/client/player/LocalPlayer.hpp>

class LevelRendererPlayer {
public:
    $padding(0x11B8);
    LocalPlayer* localPlayer;
};
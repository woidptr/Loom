#pragma once
#include "../Module.hpp"
#include <optional>
#include <cmath>
#include <sdk/GameContext.hpp>
#include <sdk/mc/client/render/game/LevelRendererPlayer.hpp>

class Zoom : public Module {
private:
    bool zoomedIn = false;
    float zoomInDuration = 1.f;
    float zoomFov = 30.f;
public:
    Zoom();

    std::optional<float> getFovCallback(LevelRendererPlayer* a1, float a2, bool a3);
    void keyboardCallback(int key, bool held);
};
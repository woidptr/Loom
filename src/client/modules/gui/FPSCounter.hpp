#pragma once
#include "../Module.hpp"

class FPSCounter : public Module {
private:
    int16_t fps = 0;
public:
    FPSCounter();

    void renderCallback();
};
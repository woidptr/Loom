#pragma once
#include "../Module.hpp"

#include <client/hooks/world/GetFovHook.hpp>

class Zoom : public Module {
public:
    Zoom(GetFovHook* getFovHook);

    void getFovCallback(void* a1, float a2, bool a3);
};
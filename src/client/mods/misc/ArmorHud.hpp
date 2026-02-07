#pragma once
#include "../Module.hpp"
#include <events/render/SetupAndRenderEvent.hpp>

class ArmorHud : public Module {
public:
    ArmorHud();

    void onRender(SetupAndRenderEvent* event);
};
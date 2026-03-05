#pragma once
#include <client/mods/Module.hpp>
#include <events/render/HudElementRenderEvent.hpp>

#include "client/Client.hpp"

class PotionHud : public Module {
public:
    PotionHud();

    void onHudElementRender(HudElementRenderEvent* event);
};

// $register_module(PotionHud);

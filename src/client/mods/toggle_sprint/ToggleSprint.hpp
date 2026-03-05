#pragma once
#include <client/mods/Module.hpp>
#include <events/render/SetupAndRenderEvent.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/render/HudElementRenderEvent.hpp>

#include "client/Client.hpp"
#include "client/mods/Setting.hpp"

class ToggleSprint : public Module {
private:
    BoolSetting<"enabled"> enabled{this, true};
    FloatSetting<"border_width"> border_width{this, 0.f};
public:
    ToggleSprint();

    void onRender(SetupAndRenderEvent* event);
    void onKey(KeyboardEvent* event);
    void onHudElementRender(HudElementRenderEvent* event);
};

$register_module(ToggleSprint);

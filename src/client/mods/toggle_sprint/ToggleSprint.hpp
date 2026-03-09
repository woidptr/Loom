#pragma once
#include <client/mods/Module.hpp>
#include <events/render/SetupAndRenderEvent.hpp>
#include <events/input/KeyboardEvent.hpp>
#include <events/render/HudElementRenderEvent.hpp>

#include "client/Client.hpp"
#include <core/settings/Setting.hpp>

class ToggleSprint : public Module {
private:
    // general settings
    BoolSetting<"enabled"> enabled{this, true};
    BoolSetting<"always_sprint"> always_sprint{this, false};

    // hud settings
    Vec2Setting<"position", "hud"> position{this, ImVec2(0.5f, 0.5f)};
    BoolSetting<"show_hud", "hud"> show_hud{this, true};
    ColorSetting<"hud_color", "hud"> hud_color{this, ImColor(0.f, 0.f, 0.f, 0.8f)};
    FloatSetting<"border_width", "hud"> border_width{this, 0.f};
    ColorSetting<"border_color", "hud"> border_color{this, ImColor(0.f, 0.f, 0.f, 0.8f)};
public:
    ToggleSprint();

    virtual bool isEnabled() const override;
    virtual void toggle() override;

    void onRender(SetupAndRenderEvent* event);
    void onKey(KeyboardEvent* event);
    void onHudElementRender(HudElementRenderEvent* event);
};

$register_module(ToggleSprint);

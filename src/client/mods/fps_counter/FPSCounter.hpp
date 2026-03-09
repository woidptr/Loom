#pragma once
#include <imgui.h>
#include <client/mods/Module.hpp>
#include <events/render/HudElementRenderEvent.hpp>
#include <core/settings/Setting.hpp>

#include "client/Client.hpp"

class FPSCounter : public Module {
private:
    BoolSetting<"enabled"> enabled{this, false};

    float borderRadius = 10.f;
public:
    FPSCounter();

    virtual bool isEnabled() const override;
    virtual void toggle() override;

    void onHudElementRender(HudElementRenderEvent* event);
};

$register_module(FPSCounter);

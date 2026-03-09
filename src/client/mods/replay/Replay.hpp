#pragma once
#include <client/mods/Module.hpp>
#include <events/network/IncomingPacketEvent.hpp>

#include "client/Client.hpp"
#include <core/settings/Setting.hpp>

class Replay : public Module {
private:
    BoolSetting<"enabled"> enabled{this, false};
public:
    Replay();

    virtual bool isEnabled() const override;
    virtual void toggle() override;

    void onPacketReceived(IncomingPacketEvent* event);
};

$register_module(Replay);

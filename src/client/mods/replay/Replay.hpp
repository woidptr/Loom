#pragma once
#include <client/mods/Module.hpp>
#include <events/network/IncomingPacketEvent.hpp>

#include "client/Client.hpp"

class Replay : public Module {
public:
    Replay();

    void onPacketReceived(IncomingPacketEvent* event);
};

$register_module(Replay);

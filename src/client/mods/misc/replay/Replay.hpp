#pragma once
#include "../../Module.hpp"
#include <events/network/IncomingPacketEvent.hpp>

class Replay : public Module {
public:
    Replay();

    void onPacketReceived(IncomingPacketEvent& event);
};
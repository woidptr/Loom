#pragma once
#include "../Module.hpp"
#include <hooks/impl/network/IncomingPacketsHook.hpp>

class Replay : public Module {
public:
    Replay();

    void onPacketReceived(IncomingPacketsEvent& event);
};
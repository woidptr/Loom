#pragma once
#include <events/EventHandler.hpp>

struct IncomingPacketEvent : public BaseEventContext {
    Packet* packet;
    MinecraftPacketIds packetId;
};
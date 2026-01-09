#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/network/MinecraftPacketIds.hpp>

class MinecraftPackets {
public:
    MCAPI static std::shared_ptr<Packet> createPacket(MinecraftPacketIds id);
};
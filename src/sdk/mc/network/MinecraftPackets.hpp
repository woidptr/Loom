#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/network/MinecraftPacketIds.hpp>

class MinecraftPackets {
public:
    /// @sig {48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 89 4C 24 ? 33 ED 81 FA}
    MCAPI static std::shared_ptr<Packet> createPacket(MinecraftPacketIds id);
};
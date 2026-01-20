#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/raknet/PacketPriority.hpp>
#include <sdk/mc/network/NetworkPeer.hpp>
#include <sdk/mc/network/Compressibility.hpp>
#include <sdk/mc/network/MinecraftPacketIds.hpp>
#include <sdk/mc/common/SubClientId.hpp>

class BinaryStream;
class ReadOnlyBinaryStream;
class IPacketHandlerDispatcher;

class Packet {
public:
    PacketPriority mPriority;
    NetworkPeer::Reliability mReliability;
    SubClientId mSenderSubId;
    bool mIsHandled;
    std::chrono::steady_clock::time_point mReceiveTimepoint;
    IPacketHandlerDispatcher const* mHandler;
    Compressibility mCompressible;
public:
    virtual ~Packet() = 0;
    virtual MinecraftPacketIds getId() const = 0;
    virtual std::string getName() const = 0;
    virtual uint64_t getMaxSize() const = 0;
    virtual void checkSize(uint64_t packetSize, bool receiveIsServer) const = 0;
    virtual void writeWithSerializationMode() const = 0;
    virtual void write() const = 0;
    virtual void write(BinaryStream& bitStream) const = 0;
    virtual void read() = 0;
    virtual void read(ReadOnlyBinaryStream& bitStream) = 0;
};
#pragma once
#include <sdk/mc/network/NetworkIdentifier.hpp>
#include <sdk/mc/network/NetEventCallback.hpp>

class Packet;

class IPacketHandlerDispatcher {
public:
    virtual ~IPacketHandlerDispatcher() = 0;
    virtual void handle(NetworkIdentifier const& networkIdentifier, NetEventCallback& callback, std::shared_ptr<Packet>& packet) const = 0;
};
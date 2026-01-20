#pragma once
#include "../../Hook.hpp"

struct IncomingPacketsEvent : public CancellableEventContext {
    Packet* packet;
    MinecraftPacketIds packet_id;
};

class TestHook : IPacketHandlerDispatcher {
public:
    void handle(void* dispatcher, NetworkIdentifier const& identifier, NetEventCallback& callback, std::shared_ptr<Packet> packet) {

    }
};

class IncomingPacketsHook {
private:
    static inline std::unordered_map<int, SafetyHookInline> hooks;
public:
    IncomingPacketsHook() {
        this->hook();
    }

    ~IncomingPacketsHook();

    static void callback(void* dispatcher, void* identifier, void* callback, std::shared_ptr<Packet>& packet) {
        MinecraftPacketIds packet_id = packet->getId();

        IncomingPacketsEvent event = IncomingPacketsEvent {
            .packet = packet.get(),
            .packet_id = packet_id,
        };

        EventHandler::emit(event);

        if (event.isCancelled()) {
            return;
        }

        return hooks[static_cast<int>(packet_id)].call<void>(dispatcher, identifier, callback, packet);
    }

    void hook() {
        for (int i = 0; i < static_cast<int>(MinecraftPacketIds::EndId); i++) {
            std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(i));
            if (!packet) continue;

            $log_debug("Found packet: {}", i);

            uintptr_t** vtablePtr = reinterpret_cast<uintptr_t**>(const_cast<IPacketHandlerDispatcher*>(packet->mHandler));
            uintptr_t* vtable = *vtablePtr;
            uintptr_t targetFunctionAddress = vtable[1];

            hooks[i] = safetyhook::create_inline((void*)targetFunctionAddress, &IncomingPacketsHook::callback);

            $log_debug("Hooked packet: {}", i);
        }
    }
};
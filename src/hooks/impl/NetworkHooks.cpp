#include "NetworkHooks.hpp"

namespace NetworkHooks {
    std::unordered_map<int, InlineHook<void(void*, void*, void*, std::shared_ptr<Packet>&)>> _incoming_packet_hooks;
    void _incoming_packet_detour(void* dispatcher, void* identifier, void* callback, std::shared_ptr<Packet>& packet) {
        MinecraftPacketIds packetId = packet->getId();

        IncomingPacketEvent event = IncomingPacketEvent{
            .packet = packet.get(),
            .packetId = packetId,
        };

        EventHandler::emit(event);

        return _incoming_packet_hooks[static_cast<int>(packetId)].call(dispatcher, identifier, callback, packet);
    }

    void init() {
        for (int i = 0; i < static_cast<int>(MinecraftPacketIds::EndId); i++) {
            std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(static_cast<MinecraftPacketIds>(i));
            if (!packet) continue;

            uintptr_t** vtablePtr = reinterpret_cast<uintptr_t**>(const_cast<IPacketHandlerDispatcher*>(packet->mHandler));
            uintptr_t* vtable = *vtablePtr;
            uintptr_t targetFunctionAddress = vtable[1];

            HookManager::createInlineHook(_incoming_packet_hooks[i].getHook(), (void*)targetFunctionAddress, &_incoming_packet_detour);
        }
    }
}
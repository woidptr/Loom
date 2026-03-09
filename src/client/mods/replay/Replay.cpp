#include "Replay.hpp"

Replay::Replay() : Module("Replay", "replay") {
    listeners.reserve(1);

    enabled.setCallback([&](bool isEnabled) -> void {
        if (isEnabled) {
            $add_listener(IncomingPacketEvent, &Replay::onPacketReceived);
        } else {
            listeners.clear();
        }
    });
}

bool Replay::isEnabled() const {
    return enabled;
}

void Replay::toggle() {
    enabled = !enabled;
}

void Replay::onPacketReceived(IncomingPacketEvent* event) {
    // $log_debug("Received packet with id: {}", static_cast<int>(event.packetId));

    // BinaryStream binaryStream = BinaryStream();

    // event.packet->write(binaryStream);

    // $log_debug("Binary stream of the packet: {}", binaryStream.mBuffer);
}
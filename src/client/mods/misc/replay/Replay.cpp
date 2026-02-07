#include "Replay.hpp"

Replay::Replay() : Module("Replay") {
    listeners.reserve(1);

    $add_listener(IncomingPacketEvent, &Replay::onPacketReceived);
}

void Replay::onPacketReceived(IncomingPacketEvent* event) {
    // $log_debug("Received packet with id: {}", static_cast<int>(event.packetId));

    // BinaryStream binaryStream = BinaryStream();

    // event.packet->write(binaryStream);

    // $log_debug("Binary stream of the packet: {}", binaryStream.mBuffer);
}
#include "Replay.hpp"

Replay::Replay() : Module("Replay") {
    listeners.reserve(1);

    listeners.emplace_back(EventHandler::subscribe<IncomingPacketsEvent, &Replay::onPacketReceived>(*this));
}

void Replay::onPacketReceived(IncomingPacketsEvent& event) {
    // $log_debug("Received packet with id: {}", static_cast<int>(event.packet_id));

    // BinaryStream binaryStream = BinaryStream();

    // event.packet->write(binaryStream);

    // $log_debug("Binary stream of the packet: {}", binaryStream.mBuffer);
}
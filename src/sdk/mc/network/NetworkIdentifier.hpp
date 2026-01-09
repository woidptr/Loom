#pragma once

class NetworkIdentifier {
public:
    enum class Type : int {
        RakNet = 0,
        Address = 1,
        Address6 = 2,
        NetherNet = 3,
        Invalid = 4,
    };
public:
    NetworkIdentifier::Type mType;
};
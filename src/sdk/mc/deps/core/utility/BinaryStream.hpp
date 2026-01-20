#pragma once
#include "ReadOnlyBinaryStream.hpp"

class BinaryStream : public ReadOnlyBinaryStream {
public:
    std::string& mBuffer;
public:
    BinaryStream() : ReadOnlyBinaryStream({}, true), mBuffer(mOwnedBuffer) {}
    BinaryStream(std::string& buffer) : ReadOnlyBinaryStream(buffer, false), mBuffer(buffer) {}
public:
    virtual ~BinaryStream() {}
};
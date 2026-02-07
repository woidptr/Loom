#pragma once
#include "ReadOnlyBinaryStream.hpp"

class BinaryStream : public ReadOnlyBinaryStream {
public:
    std::string& mBuffer;
public:
    BinaryStream() : ReadOnlyBinaryStream({}, true), mBuffer(mOwnedBuffer) {
        this->vtable = $get_address("BinaryStream::$vftable");
    }
    // BinaryStream(std::string& buffer) : ReadOnlyBinaryStream(buffer, false), mBuffer(buffer) {}

    /*virtual ~BinaryStream() = 0;
    virtual void writeBool(bool value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeByte(unsigned char value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeUnsignedShort(unsigned short value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeSignedShort(short value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeUnsignedInt(unsigned int value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeSignedBigEndianInt(int value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeSignedInt(int value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeUnsignedInt64(uint64_t value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeSignedInt64(int64_t value, const char* docFieldName, const char* docFieldNotes) = 0;
    virtual void writeUnsignedVarInt(unsigned int uvalue, const char* docFieldName, const char* docFieldNotes) = 0;*/
};
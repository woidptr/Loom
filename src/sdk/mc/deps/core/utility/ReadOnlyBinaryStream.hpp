#pragma once
#include <sdk/Predefine.hpp>

class ReadOnlyBinaryStream {
public:
    std::string mOwnedBuffer;
    std::string_view mView;
    uint64_t mReadPointer;
    bool mHasOverflowed;
public:
    ReadOnlyBinaryStream(std::string_view buffer, bool copyBuffer)
        : mView(buffer), mReadPointer(0), mHasOverflowed(false) {
            if (copyBuffer) {
                mOwnedBuffer = std::string(buffer);
                mView = mOwnedBuffer;
            }
        }
public:
    virtual ~ReadOnlyBinaryStream() {}
    virtual void read(void* target, uint64_t num) {}
};
#pragma once
#include <sdk/Predefine.hpp>

class ReadOnlyBinaryStream {
protected:
    uintptr_t vtable;
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

    /*virtual ~ReadOnlyBinaryStream() = 0;
    virtual void read() = 0;*/
};
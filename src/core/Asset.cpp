#include "Asset.hpp"

extern "C" {
    typedef struct RawAsset {
        const uint8_t* data;
        size_t size;
    } RawAsset;

    RawAsset get_asset(const char* name);
}

Asset::Asset(std::string_view name) {
    RawAsset asset = get_asset(name.data());

    mData = asset.data;
    mSize = asset.size;
}

const uint8_t* Asset::data() const noexcept {
    return mData;
}

size_t Asset::size() const noexcept {
    return mSize;
}

std::string_view Asset::str() const noexcept {
    return std::string_view(reinterpret_cast<const char*>(mData), mSize);
}
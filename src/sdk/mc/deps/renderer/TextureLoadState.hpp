#pragma once

enum class TextureLoadState : unsigned char {
    UnloadedBit = 0,
    PendingBit = 1 << 0,
    LoadedBit = 1 << 1,
    PendingMetadata = 1 << 2,
    LoadedMetadata = 1 << 3,
};
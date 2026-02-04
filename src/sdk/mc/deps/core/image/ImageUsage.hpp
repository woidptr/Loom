#pragma once

namespace mce {
    enum class ImageUsage : unsigned char {
        Unknown = 0,
        SRGB    = 1,
        Data    = 2,
    };
}
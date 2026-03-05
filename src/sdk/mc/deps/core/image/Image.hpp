#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core/image/ImageFormat.hpp>
#include <sdk/mc/deps/core/image/ImageUsage.hpp>

namespace mce {
    struct Image {
        mce::ImageFormat imageFormat;
        uint32_t mWidth;
        uint32_t mHeight;
        uint32_t mDepth;
        mce::ImageUsage mUsage;
    };
}
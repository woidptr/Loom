#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core/image/ImageFormat.hpp>
#include <sdk/mc/deps/core/image/ImageUsage.hpp>

namespace mce {
    struct Image {
        mce::ImageFormat imageFormat;
        unsigned int mWidth;
        unsigned int mHeight;
        unsigned int mDepth;
        mce::ImageUsage mUsage;
    };
}
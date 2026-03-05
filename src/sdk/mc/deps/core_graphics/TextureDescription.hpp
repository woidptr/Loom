#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core_graphics/ImageDescription.h>

namespace cg {
    struct TextureDescription : public cg::ImageDescription {
    public:
        uint32_t mMipCount;
    };
}
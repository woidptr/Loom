#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core_graphics/enums/TextureFormat.hpp>
#include <sdk/mc/deps/core_graphics/enums/ColorSpace.hpp>
#include <sdk/mc/deps/core_graphics/ImageType.hpp>

namespace cg {
    struct ImageDescription {
    public:
        uint32_t mWidth{0};
        uint32_t mHeight{0};
        mce::TextureFormat mTextureFormat;
        cg::ColorSpace mColorSpace{cg::ColorSpace::Unknown};
        cg::ImageType mImageType{cg::ImageType::Texture2D};
    };
}
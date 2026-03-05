#pragma once

namespace cg {
    enum class ImageType : unsigned char {
        Texture2D = 0,
        CubemapDeprecated = 1,
        Texture3D = 2,
        TextureCube = 3,
    };
}
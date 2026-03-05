#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>
#include <sdk/mc/deps/core_graphics/TextureDescription.hpp>

namespace mce {
    struct TextureDescription : public cg::TextureDescription {
    public:
        mce::Color mClearColor;
        float mOptimizedClearDepth;
        unsigned char mOptimizedClearStencil;
    };
}
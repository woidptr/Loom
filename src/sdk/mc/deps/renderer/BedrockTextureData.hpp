#pragma once
#include <core/Memory.hpp>
#include <sdk/mc/deps/core_graphics/TextureDescription.hpp>
#include <sdk/mc/deps/renderer/IsMissingTexture.hpp>
#include <sdk/mc/deps/renderer/TextureLoadState.hpp>

struct BedrockTextureData {
public:
    $padding(88);

    IsMissingTexture mIsMissingTexture;
    TextureLoadState mTextureLoadState;

    $padding(24);
};
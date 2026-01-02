#pragma once
#include <core/Memory.hpp>
#include <sdk/mc/deps/renderer/IsMissingTexture.hpp>
#include <sdk/mc/deps/renderer/TextureLoadState.hpp>

class BedrockTextureData {
public:
    $padding(88);

    IsMissingTexture mIsMissingTexture;
    TextureLoadState mTextureLoadState;

    $padding(24);
};
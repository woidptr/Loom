#pragma once
#include <memory>
#include <core/Memory.hpp>
#include <sdk/mc/deps/renderer/BedrockTextureData.hpp>
#include <sdk/mc/deps/core/resource/ResourceLocation.hpp>

namespace mce {
	class TexturePtr {
	public:
		std::shared_ptr<BedrockTextureData> mClientTexture;
		std::shared_ptr<ResourceLocation> mResourceLocation;
	public:
		TexturePtr(const ResourceLocation& resourceLocation);
		TexturePtr() {}
	};
}
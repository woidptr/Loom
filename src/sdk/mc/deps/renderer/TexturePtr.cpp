#include "TexturePtr.hpp"

mce::TexturePtr::TexturePtr(const ResourceLocation& resourceLocation) {
	this->mClientTexture = nullptr;
	this->mResourceLocation = std::make_shared<ResourceLocation>(resourceLocation);
}
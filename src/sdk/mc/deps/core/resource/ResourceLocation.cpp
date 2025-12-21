#include "ResourceLocation.hpp"

ResourceLocation::ResourceLocation(const std::string& path, ResourceFileSystem fileSystem) {
	this->mFileSystem = ResourceFileSystem::Raw;
	this->mPath = path;

	uint64_t hash = HashedString::computeHash(mPath);

	// this->mPathHash = hash;
	// this->mFullHash = hash ^ static_cast<uint64_t>(mFileSystem);
	// this->mFullHash = (0x100000001B3 * ((uint8_t)this->mFileSystem ^ 0xCBF29CE484222325)) ^ hash;
	_computeHashes();
}

//ResourceLocation& ResourceLocation::operator=(const ResourceLocation& other) {
//	if (this != &other) {
//		mFileSystem = other.mFileSystem;
//		mPath = other.mPath;
//		mPathHash = other.mPathHash;
//		mFullHash = other.mFullHash;
//	}
//	return *this;
//}
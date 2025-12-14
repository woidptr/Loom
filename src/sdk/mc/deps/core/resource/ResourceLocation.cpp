#include "ResourceLocation.hpp"

ResourceLocation::ResourceLocation(const std::string& path) {
	this->mFileSystem = ResourceFileSystem::Raw;
	this->mPath = path;
	/*this->mPathHash = HashedString(path).mStrHash;

	uint64_t fsHash = 0xCBF29CE484222325ULL ^ (uint8_t)this->mFileSystem;
	fsHash *= 0x100000001B3ULL;

	this->mFullHash = this->mPathHash ^ fsHash;*/

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
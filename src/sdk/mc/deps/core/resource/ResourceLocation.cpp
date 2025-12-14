#include "ResourceLocation.hpp"

ResourceLocation::ResourceLocation(const std::string& path) {
	this->mFileSystem = ResourceFileSystem::Raw;
	this->mPath = path;
	this->mPathHash = HashedString(path).mStrHash;

	uint64_t fsHash = 0xCBF29CE484222325ULL;
	fsHash *= 0x100000001B3LL;
	fsHash ^= (uint8_t)this->mFileSystem;

	this->mFullHash = this->mPathHash ^ fsHash;
}

ResourceLocation::ResourceLocation(const ResourceLocation& other)
	: mFileSystem(other.mFileSystem),
	mPath(other.mPath),
	mPathHash(other.mPathHash),
	mFullHash(other.mFullHash) { }

ResourceLocation& ResourceLocation::operator=(const ResourceLocation& other) {
	if (this != &other) {
		mFileSystem = other.mFileSystem;
		mPath = other.mPath;
		mPathHash = other.mPathHash;
		mFullHash = other.mFullHash;
	}
	return *this;
}
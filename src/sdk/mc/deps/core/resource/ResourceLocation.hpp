#pragma once
#include <sdk/mc/deps/core/resource/ResourceFileSystem.hpp>
#include <sdk/mc/deps/core/string/HashedString.hpp>

class ResourceLocation {
public:
	ResourceFileSystem mFileSystem;
	std::string mPath;
	uint64_t mPathHash;
	uint64_t mFullHash;
public:
	ResourceLocation(const std::string& path);
	// ResourceLocation& operator=(const ResourceLocation& other);
private:
    void _computeHashes()
    {
        const uint64_t FNV_OFFSET_BASIS = 0xCBF29CE484222325u;
        const uint64_t FNV_PRIME = 0x100000001B3u;

        uint64_t _pathHash = FNV_OFFSET_BASIS;
        for (char c : this->mPath) {
            _pathHash *= FNV_PRIME;
            _pathHash ^= c;
        }

        uint64_t fileSystemHash = FNV_OFFSET_BASIS ^ static_cast<uint8_t>(this->mFileSystem);
        fileSystemHash *= FNV_PRIME;

        this->mPathHash = _pathHash;
        this->mFullHash = _pathHash ^ fileSystemHash;
    }
};
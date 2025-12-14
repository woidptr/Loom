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
	ResourceLocation(const ResourceLocation& other);
	ResourceLocation& operator=(const ResourceLocation& other);
};
#pragma once
#include <cstdint>
#include <string>

class HashedString {
public:
	uint64_t mStrHash;
	std::string mStr;
	HashedString* mLastMatch;
public:
	HashedString(const std::string& str);
};
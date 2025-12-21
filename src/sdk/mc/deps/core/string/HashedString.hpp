#pragma once
#include <cstdint>
#include <string>

class HashedString {
public:
	uint64_t mStrHash;
	std::string mStr;
	HashedString* mLastMatch;
public:
	HashedString(std::string str) : mStrHash(computeHash(str)), mStr(str) {}

	static constexpr uint64_t computeHash(std::string_view str) {
		if (str.empty()) {
			return 0;
		}

		uint64_t hash = 0xCBF29CE484222325;
		for (const char c : str) {
			hash *= 0x100000001B3;
			hash ^= c;
		}

		return hash;
	}
};
#include "HashedString.hpp"

HashedString::HashedString(const std::string& str) {
	uint64_t hash = 0xCBF29CE484222325ULL;
	for (unsigned char c : str) {
		hash ^= c;
		hash *= 0x100000001B3ULL;
	}

	this->mStrHash = hash;
}
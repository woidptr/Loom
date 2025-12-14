#include "HashedString.hpp"

HashedString::HashedString(const std::string& str) {
	const uintptr_t fnvOffsetBasis = 0xCBF29CE484222325ull;
	const uintptr_t fnvPrime = 0x100000001B3ull;

	uint64_t hash = fnvOffsetBasis;
	for (unsigned char c : str) {
		hash *= fnvPrime;
		hash ^= c;
	}

	this->mStrHash = hash;
}
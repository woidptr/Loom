#pragma once
#include <string>
#include <vector>
#include <libhat/scanner.hpp>

#include "Signatures.h"

template <typename SigType>
class Signature {
public:
	std::string name;
	SigType signature;
	int16_t offset = 0;

	constexpr Signature(std::string name, SigType signature, int16_t offset) {
		this->name = name;
		this->signature = signature;
		this->offset = offset;
	}

	uintptr_t getAddress() {
		hat::scan_result result = hat::find_pattern(this->signature, ".text");

		std::byte* address;

		if (offset == 0) {
			address = result.get();
		}
		else {
			address = result.rel(offset);
		}

		return address;
	}
};

class SignatureManager {
public:
	SignatureManager();
};

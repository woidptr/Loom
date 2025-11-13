#pragma once
#include <libhat/scanner.hpp>

class Signature {
private:
	uintptr_t address;

public:
	Signature(std::string signature, int16_t offset) {
		using parsed_t = hat::result<hat::signature, hat::signature_error>;
		parsed_t pattern = hat::parse_signature(signature);

		hat::scan_result result = hat::find_pattern(pattern.value(), ".text");
		if (offset == 0) {
			this->address = (uintptr_t)result.get();
		}
		else {
			this->address = (uintptr_t)result.rel(offset);
		}
	}

	uintptr_t getAddress() {
		return this->address;
	}
};

namespace Signatures {
	namespace Tessellator {
		inline Signature begin_ = Signature("E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75", 1);
	}

	namespace ScreenView {
		inline Signature setupAndRender = Signature("48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 BC 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4C 8B FA", 0);
	}
}

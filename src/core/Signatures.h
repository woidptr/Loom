#pragma once
#include <libhat/scanner.hpp>

namespace Signatures {
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

	namespace Tessellator {
		/*inline constexpr Signature<decltype(hat::compile_signature<"E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75">())> begin_{
			"Tessellator::begin",
			hat::compile_signature<"E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75">(),
			0
		};*/
		inline constexpr hat::fixed_signature begin = hat::compile_signature<"E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75">();
	}
}

#include "Tessellator.h"
#include <bit>
#include <libhat/scanner.hpp>

#include "../../core/SignatureManager.h"

void Tessellator::begin(int a1, int maxVertices) {
	using _func = void(*)(Tessellator*, int, int);

	constexpr hat::fixed_signature pattern = hat::compile_signature<"E8 ? ? ? ? F3 41 0F 10 4C 24 ? F3 41 0F 10 04 24 F3 0F 11 45 ? F3 0F 11 75">();
	hat::scan_result result = hat::find_pattern(pattern, ".text");
	std::byte* address = result.rel(1);

	_func func = reinterpret_cast<_func>(address);

	// _func func = SignatureManager::FuncFromSigOffset<_func>((uintptr_t)address, 1);

	return func(this, a1, maxVertices);
}

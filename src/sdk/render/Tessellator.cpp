#include "Tessellator.h"
#include <bit>
#include <libhat/scanner.hpp>

#include "../../core/Signatures.h"

void Tessellator::begin(int a1, int maxVertices) {
	using _func = void(*)(Tessellator*, int, int);

	uintptr_t address = Signatures::Tessellator::begin_.getAddress();

	_func func = reinterpret_cast<_func>(address);

	return func(this, a1, maxVertices);
}

#include "Tessellator.h"
#include <bit>
#include <libhat/scanner.hpp>

#include "../../core/Signatures.h"

void Tessellator::begin(int a1, int maxVertices) {
	using _func = void(*)(Tessellator*, int, int);

	_func func = reinterpret_cast<_func>(Signatures::Tessellator::begin.getAddress());

	return func(this, a1, maxVertices);
}

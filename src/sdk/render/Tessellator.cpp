#include "Tessellator.hpp"

void Tessellator::begin(int a1, int maxVertices) {
	return reinterpret_cast<decltype(+[](Tessellator*, int, int) -> void {}) > (SignatureRegistry::getSignature("Tessellator::begin"))(this, a1, maxVertices);
}

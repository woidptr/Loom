#include "Tessellator.hpp"
#include <libhat.hpp>

void Tessellator::begin(int a1, int maxVertices) {
	return reinterpret_cast<decltype(+[](Tessellator*, int, int) -> void {}) > (SignatureRegistry::getSignature("Tessellator::begin"))(this, a1, maxVertices);
}

void Tessellator::color(mce::Color color) {
    struct PackedColors {
        union {
            struct {
                char r;
                char g;
                char b;
                char a;
            };
            unsigned int intValue;
        };
    };

    PackedColors result{};
    result.r = static_cast<char>(color.r * 255.0f);
    result.g = static_cast<char>(color.g * 255.0f);
    result.b = static_cast<char>(color.b * 255.0f);
    result.a = static_cast<char>(color.a * 255.0f);

    this->mNextColor = result.intValue;
}



mce::Mesh Tessellator::end(int uploadMode, std::string_view debugName, int generatedNormals) {
	return (this->*[] {static auto f = std::bit_cast<decltype(&Tessellator::end)>(SignatureRegistry::getSignature("Tessellator::end")); return f; }()) (uploadMode, debugName, generatedNormals);
}
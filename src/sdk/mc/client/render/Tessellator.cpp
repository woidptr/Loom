#include "Tessellator.hpp"
#include <libhat.hpp>

void Tessellator::begin(mce::PrimitiveMode mode, int maxVertices, bool buildFaceData) {
    // return Memory::CallMember<decltype(&Tessellator::begin)>($get_address("Tessellator::begin"), *this, mode, maxVertices, buildFaceData);
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

void Tessellator::color(uint32_t color) {
    this->mNextColor = color;
}

void Tessellator::vertex(float x, float y, float z) {
    // return Memory::CallMember<decltype(&Tessellator::vertex)>($getAddress("Tessellator::vertex"), *this, x, y, z);
}

void Tessellator::vertexUV(Vec3 pos, Vec2 nextUV) {
    this->mNextUV[0] = nextUV;
    this->vertex(pos.x, pos.y, pos.z);
}

//mce::Mesh Tessellator::end(int uploadMode, std::string_view debugName, int generatedNormals) {
//    return Memory::CallMember<decltype(&Tessellator::end)>($getAddress("Tessellator::end"), *this, uploadMode, debugName, generatedNormals);
//}
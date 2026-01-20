#include "Tessellator.hpp"
#include <libhat.hpp>

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

void Tessellator::vertexUV(Vec3 pos, Vec2 nextUV) {
    // this->mNextUV[0] = nextUV;
    // this->vertex(pos.x, pos.y, pos.z);
}
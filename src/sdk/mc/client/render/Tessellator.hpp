#pragma once
#include <string>
#include <core/Signatures.hpp>
#include <sdk/mc/client/render/Mesh.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>
#include <sdk/mc/deps/core/math/Vec2.hpp>
#include <sdk/mc/deps/core/math/Vec3.hpp>
#include <sdk/mc/deps/core_graphics/enums/PrimitiveMode.hpp>
#include <core/Memory.hpp>

class Tessellator {
public:
	// $padding(0x170);
	$padding(0x14C);
	std::optional<Vec2> mNextUV[3];
	// std::optional<uint32_t> mNextColor;
	$field(std::optional<uint32_t>, mNextColor);
public:
	void begin(mce::PrimitiveMode mode, int maxVertices, bool buildFaceData);
	void color(mce::Color color);
	void color(uint32_t color);
	void vertex(float x, float y, float z);
	void vertexUV(Vec3 pos, Vec2 nextUV);
	// mce::Mesh end(int uploadMode, std::string_view debugName, int generateNormals);
};

#pragma once
#include <string>
#include <core/Signatures.hpp>
#include <sdk/mc/client/render/Mesh.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>
#include <core/Memory.hpp>

class Tessellator {
public:
	$field(std::optional<uint32_t>, mNextColor, 0x170);
	// std::byte padding372[0x170];
	// std::optional<uint32_t> mNextColor;
public:
	void begin(int a1, int maxVertices);
	void color(mce::Color color);
	mce::Mesh end(int uploadMode, std::string_view debugName, int generateNormals);
};

#pragma once
#include <string>
#include <core/Signatures.hpp>
#include <sdk/mc/client/render/Mesh.h>
#include <sdk/mc/deps/core/math/Color.h>

class Tessellator {
public:
	std::byte padding372[0x170];
	std::optional<uint32_t> mNextColor;
public:
	void begin(int a1, int maxVertices);
	void color(mce::Color color);
	mce::Mesh end(int uploadMode, std::string_view debugName, int generateNormals);
};

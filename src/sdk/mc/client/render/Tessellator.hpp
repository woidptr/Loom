#pragma once
#include <sdk/Predefine.hpp>
#include <sdk/mc/deps/core/math/Color.hpp>
#include <sdk/mc/deps/core/math/Vec2.hpp>
#include <sdk/mc/deps/core/math/Vec3.hpp>
#include <sdk/mc/deps/core_graphics/enums/PrimitiveMode.hpp>

namespace mce {
	class Mesh;
}

class Tessellator {
public:
	enum class UploadMode : int {
		Buffered = 0,
		Unbuffered = 1,
	};

	enum class SupplementaryFieldAutoGenerationMode : int {
		idk = 0,
	};
public:
	$padding(0x14C);
	std::optional<Vec2> mNextUV[3];

	$field(std::optional<uint32_t>, mNextColor)
public:
	MCAPI void begin(mce::PrimitiveMode mode, int maxVertices, bool buildFaceData);
	MCAPI void vertex(float x, float y, float z);
	MCAPI mce::Mesh end(UploadMode uploadMode, std::string_view debugName, SupplementaryFieldAutoGenerationMode generateNormals);

	void color(mce::Color color);
	void color(uint32_t color);
	void vertexUV(Vec3 pos, Vec2 nextUV);
};

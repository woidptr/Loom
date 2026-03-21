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
	// member fields
	$padding(0x14C);
	std::optional<Vec2> mNextUV[3];

	/// @sig {89 86 ? ? ? ? 44 38 36 75 ? C6 86 ? ? ? ? ? 0F 57 DB 0F 57 D2}
	/// @type field
	uint32_t& mNextColor();
public:
	// member functions

	/// @sig {40 57 48 83 EC ? 80 B9 ? ? ? ? ? 4C 8B D1}
	MCAPI void begin(mce::PrimitiveMode mode, int maxVertices, bool buildFaceData);

	/// @sig {E8 ? ? ? ? F3 0F 10 4E ? 0F 2F F9}
	MCAPI void vertex(float x, float y, float z);

	/// @sig {E8 ? ? ? ? 48 8D 55 ? 49 8B CC E8 ? ? ? ? 48 8D 4D ? E8 ? ? ? ? F3 45 0F 11 65}
	MCAPI mce::Mesh end(UploadMode uploadMode, std::string_view debugName, SupplementaryFieldAutoGenerationMode generateNormals);

	void color(mce::Color color);
	void color(uint32_t color);
	void vertexUV(Vec3 pos, Vec2 nextUV);
};

class TessellatorWrapper {
private:
	Tessellator* tessellator;
public:
	Tessellator* operator->() const {
		return tessellator;
	}

	operator Tessellator*() const {
		return tessellator;
	}

	Tessellator& operator*() const {
		return *tessellator;
	}
};

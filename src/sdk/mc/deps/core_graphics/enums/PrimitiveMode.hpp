#pragma once
#include <cstdint>

namespace mce {
	enum class PrimitiveMode : uint8_t {
		None          = 0,
		QuadList      = 1,
		TriangleList  = 2,
		TriangleStrip = 3,
		LineList      = 4,
		LineStrip     = 5,
	};
}
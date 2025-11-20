#pragma once

namespace mce {
	struct Color {
		float r;
		float g;
		float b;
		float a;

		Color(float r, float g, float b, float a)
			: r(r), g(g), b(b), a(a) { }
	};
}
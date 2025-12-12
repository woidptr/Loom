#pragma once

namespace mce {
	class Color {
	public:
		float r;
		float g;
		float b;
		float a;

	public:
		Color(float r, float g, float b, float a)
			: r(r), g(g), b(b), a(a) { }
	};
}
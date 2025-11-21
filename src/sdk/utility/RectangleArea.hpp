#pragma once

struct RectangleArea {
	float x1;
	float x2;
	float y1;
	float y2;

	RectangleArea(float x1, float x2, float y1, float y2)
		: x1(x1), x2(x2), y1(y1), y2(y2) { }

	void set(float x1, float x2, float y1, float y2) {
		*this = RectangleArea(x1, x2, y1, y2);
	}
};
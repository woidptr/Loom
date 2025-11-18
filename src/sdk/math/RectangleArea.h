#pragma once

class RectangleArea {
public:
	float x1;
	float x2;
	float y1;
	float y2;
public:
	RectangleArea(float x1, float x2, float y1, float y2)
		: x1(x1), x2(x2), y1(y1), y2(y2) = 0;

	void set(float x1, float x2, float y1, float y2) {
		this->x1 = x1;
		this->x2 = x2;
		this->y1 = y1;
		this->y2 = y2;
	}
};
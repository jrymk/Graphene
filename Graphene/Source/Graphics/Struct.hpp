#pragma once


struct Vec2u {
	unsigned int x = 0.0f;
	unsigned int y = 0.0f;

	Vec2u() = default;

	Vec2u(unsigned X, unsigned Y) {
		x = X;
		y = Y;
	}
};


struct Vec2f {
	float x = 0.0f;
	float y = 0.0f;

	Vec2f() = default;

	Vec2f(float X, float Y) {
		x = X;
		y = Y;
	}
};
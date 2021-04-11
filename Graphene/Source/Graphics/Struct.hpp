#pragma once


struct Vec2u {
	unsigned int x = 0;
	unsigned int y = 0;

	Vec2u() = default;

	Vec2u(unsigned int X, unsigned int Y) {
		x = X;
		y = Y;
	}
};

struct Vec2i {
	int x = 0;
	int y = 0;

	Vec2i() = default;

	Vec2i(int X, int Y) {
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
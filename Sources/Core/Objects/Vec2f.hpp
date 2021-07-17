#pragma once

#include <iostream>
#include <cmath>

namespace gfn::core {
class Vec2f {
  public:
	double x = 0.0;
	double y = 0.0;

	Vec2f() = default;

	Vec2f(double _x, double _y) {
		x = _x;
		y = _y;
	}

	Vec2f operator+(Vec2f a) const { return {this->x + a.x, this->y + a.y}; }

	void operator+=(Vec2f a) {
		this->x += a.x;
		this->y += a.y;
	}

	Vec2f operator-(Vec2f a) const { return {this->x - a.x, this->y - a.y}; }

	void operator-=(Vec2f a) {
		this->x -= a.x;
		this->y -= a.y;
	}

	// linear multiply
	Vec2f operator*(double a) const { return {this->x * a, this->y * a}; }

	void operator*=(double a) {
		this->x *= a;
		this->y *= a;
	}

	// linear divide
	Vec2f operator/(double a) const { return {this->x / a, this->y / a}; }

	void operator/=(double a) {
		this->x /= a;
		this->y /= a;
	}

	// dot product
	double operator*(Vec2f a) const { return this->x * a.x + this->y * a.y; }

	// cross product
	double operator^(Vec2f a) const { return this->x * a.y - this->y * a.x; }

	// absolute (length)
	double length() const { return sqrt(this->x * this->x + this->y * this->y); }

	static double length(const Vec2f& a) { return sqrt(a.x * a.x + a.y * a.y); }

	static double distance(const Vec2f& a, const Vec2f& b) {
		return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}

	Vec2f rotate(double theta) const {
		double cosTheta = cos(theta);
		double sinTheta = sin(theta);
		return {x * cosTheta + y * sinTheta, x * (-sinTheta) + y * cosTheta};
	}

	Vec2f normalize() {
		if (length() <= 1e-9)
			return {0, 0};
		return Vec2f(*this) / length();
	}

	bool operator<(const Vec2f& a) const {
		if (this->x == a.x)
			return this->y < a.y;
		return this->x < a.x;
	}

	bool operator<=(const Vec2f& a) const {
		if (this->x == a.x)
			return this->y <= a.y;
		return this->x < a.x;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec2f& nd) {
		return os << "(" << nd.x << ", " << nd.y << ")";
	}
};
} // namespace gfn::core
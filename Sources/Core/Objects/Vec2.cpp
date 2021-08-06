#include "Vec2.h"

namespace gfn {
    Vec2::Vec2() = default;

    Vec2::Vec2(double _x, double _y) {
        x = _x;
        y = _y;
    }

    Vec2 Vec2::operator+(Vec2 a) const { return {this->x + a.x, this->y + a.y}; }

    void Vec2::operator+=(Vec2 a) {
        this->x += a.x;
        this->y += a.y;
    }

    Vec2 Vec2::operator-(Vec2 a) const { return {this->x - a.x, this->y - a.y}; }

    void Vec2::operator-=(Vec2 a) {
        this->x -= a.x;
        this->y -= a.y;
    }

    // linear multiply
    Vec2 Vec2::operator*(double a) const { return {this->x * a, this->y * a}; }

    void Vec2::operator*=(double a) {
        this->x *= a;
        this->y *= a;
    }

    // linear divide
    Vec2 Vec2::operator/(double a) const { return {this->x / a, this->y / a}; }

    void Vec2::operator/=(double a) {
        this->x /= a;
        this->y /= a;
    }

    // dot product
    double Vec2::operator*(Vec2 a) const { return this->x * a.x + this->y * a.y; }

    // cross product
    double Vec2::operator^(Vec2 a) const { return this->x * a.y - this->y * a.x; }

    // absolute (length)
    double Vec2::length() const { return sqrt(this->x * this->x + this->y * this->y); }

    double Vec2::length(const Vec2& a) { return sqrt(a.x * a.x + a.y * a.y); }

    double Vec2::distance(const Vec2& a, const Vec2& b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    Vec2 Vec2::rotate(double theta) const {
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        return {x * cosTheta + y * sinTheta, x * (-sinTheta) + y * cosTheta};
    }

    Vec2 Vec2::normalize() {
        if (length() <= 1e-9)
            return {0, 0};
        return Vec2(*this) / length();
    }

    bool Vec2::operator<(const Vec2& a) const {
        if (this->x == a.x)
            return this->y < a.y;
        return this->x < a.x;
    }

    bool Vec2::operator<=(const Vec2& a) const {
        if (this->x == a.x)
            return this->y <= a.y;
        return this->x < a.x;
    }

    std::ostream& operator<<(std::ostream& os, const Vec2& nd) {
        return os << "(" << nd.x << ", " << nd.y << ")";
    }
}
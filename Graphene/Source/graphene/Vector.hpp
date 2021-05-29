#pragma once

namespace Graphene {

    struct Vec2f {
        double x;
        double y;

        Vec2f(double _x, double _y) {
            x = _x;
            y = _y;
        }

        /********* Operator Definitions *********
        *|	+ add
        *|	- subtract
        *|	* mul by number
        *|	/ divide by number
        *|	* dot
        *|	^ cross
        *****************************************/

        Vec2f operator+(Vec2f a) const {
            return {this->x + a.x, this->y + a.y};
        }

        void operator+=(Vec2f a) {
            this->x += a.x;
            this->y += a.y;
        }

        Vec2f operator-(Vec2f a) const {
            return {this->x - a.x, this->y - a.y};
        }

        void operator-=(Vec2f a) {
            this->x -= a.x;
            this->y -= a.y;
        }

        Vec2f operator*(double a) const {
            return {this->x * a, this->y * a};
        }

        void operator*=(double a) {
            this->x *= a;
            this->y *= a;
        }

        Vec2f operator/(double a) const {
            return {this->x / a, this->y / a};
        }

        void operator/=(double a) {
            this->x /= a;
            this->y /= a;
        }

        double operator*(Vec2f a) const {
            return this->x * a.x + this->y * a.y;
        }

        double operator^(Vec2f a) const {
            return this->x * a.y - this->y * a.x;
        }

        double length() const {
            return sqrt(this->x * this->x + this->y * this->y);
        }

        static double length(const Vec2f &a) {
            return sqrt(a.x * a.x + a.y * a.y);
        }

        static double distance(const Vec2f &a, const Vec2f &b) {
            return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        }

        Vec2f rotate(double theta) const {
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);
            return {x * cosTheta + y * sinTheta, x * (-sinTheta) + y * cosTheta};
        }

        Vec2f normalize() {
            if (length() <= 1e-9) return {0, 0};
            return Vec2f(*this) / length();
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec2f &nd) {
            return os << "(" << nd.x << ", " << nd.y << ")";
        }
    };

}
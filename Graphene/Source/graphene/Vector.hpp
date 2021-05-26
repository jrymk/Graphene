#pragma once

namespace Graphene {

    struct Vec2f {
        float x;
        float y;

        Vec2f(float _x, float _y) {
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

        Vec2f operator+(Vec2f a) {
            return Vec2f(this->x + a.x, this->y + a.y);
        }

        void operator+=(Vec2f a) {
            this->x += a.x;
            this->y += a.y;
        }

        Vec2f operator-(Vec2f a) {
            return Vec2f(this->x - a.x, this->y - a.y);
        }

        void operator-=(Vec2f a) {
            this->x -= a.x;
            this->y -= a.y;
        }

        Vec2f operator*(float a) {
            return Vec2f(this->x * a, this->y * a);
        }

        void operator*=(float a) {
            this->x *= a;
            this->y *= a;
        }

        Vec2f operator/(float a) {
            return Vec2f(this->x / a, this->y / a);
        }

        void operator/=(float a) {
            this->x /= a;
            this->y /= a;
        }

        float operator*(Vec2f a) {
            return this->x * a.x + this->y * a.y;
        }

        float operator^(Vec2f a) {
            return this->x * a.y - this->y * a.x;
        }

        float length() {
            return sqrt(this->x * this->x + this->y * this->y);
        }

        Vec2f rotate(double theta) {
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);
            return Vec2f(x * cosTheta + y * sinTheta, x * (-sinTheta) + y * cosTheta);
        }

        Vec2f normalize() {
            if (length() <= 1e-9) return Vec2f(0, 0);
            return Vec2f(*this) / length();
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec2f &nd) {
            return os << "(" << nd.x << ", " << nd.y << ")";
        }
    };

}
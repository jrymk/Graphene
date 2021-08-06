#ifndef VEC2F_H
#define VEC2F_H

#include <iostream>
#include <cmath>

namespace gfn {
    class Vec2 {
    public:
        double x = 0.0;
        double y = 0.0;

        Vec2();

        Vec2(double _x, double _y);

        Vec2 operator+(Vec2 a) const;

        void operator+=(Vec2 a);

        Vec2 operator-(Vec2 a) const;

        void operator-=(Vec2 a);

        // linear multiply
        Vec2 operator*(double a) const;

        void operator*=(double a);

        // linear divide
        Vec2 operator/(double a) const;

        void operator/=(double a);

        // dot product
        double operator*(Vec2 a) const;

        // cross product
        double operator^(Vec2 a) const;

        // absolute (length)
        double length() const;

        static double length(const Vec2& a);

        static double distance(const Vec2& a, const Vec2& b);

        Vec2 rotate(double theta) const;

        Vec2 normalize();

        bool operator<(const Vec2& a) const;

        bool operator<=(const Vec2& a) const;

        friend std::ostream& operator<<(std::ostream& os, const Vec2& nd);
    };
}

#endif
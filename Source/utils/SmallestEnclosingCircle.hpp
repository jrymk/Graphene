#include <vector>
#include "../graphene/Structure.hpp"

namespace Utils {

    class SmallestEnclosingCircle {
    private:
        std::vector<Graphene::Vec2f> points;

        float lengthSq(Graphene::Vec2f p) {
            return p.x * p.x + p.y * p.y;
        }

        bool outside(Graphene::Vec2f p, Graphene::Vec2f c, float rSq) {
            return lengthSq(p - c) > rSq;
        }

        float cross(Graphene::Vec2f v1, Graphene::Vec2f v2) {
            return v1.x * v2.y - v1.y * v2.x;
        }

        std::pair<Graphene::Vec2f, float> makeCircle(Graphene::Vec2f p1, Graphene::Vec2f p2) {
            return {(p1 + p2) / 2.0f, lengthSq(p1 - p2) / 4.0f};
        }

        std::pair<Graphene::Vec2f, float> makeCircle(Graphene::Vec2f p1, Graphene::Vec2f p2, Graphene::Vec2f p3) {
            Graphene::Vec2f c(0.0f, 0.0f);
            float rSq = 0.0f;
            Graphene::Vec2f vA = {p1.y - p2.y, p2.x - p1.x};
            Graphene::Vec2f vB = {p1.y - p3.y, p3.x - p1.x};

            if (abs(cross(vA, vB)) < 1E-5) {
                Graphene::Vec2f tC(0.0f, 0.0f);
                float tRSq = 0.0f;
                std::tie(c, rSq) = makeCircle(p1, p2);
                std::tie(tC, tRSq) = makeCircle(p1, p3);
                if (tRSq > rSq) {
                    c = tC;
                    rSq = tRSq;
                }
                std::tie(tC, tRSq) = makeCircle(p2, p3);
                if (tRSq > rSq) {
                    c = tC;
                    rSq = tRSq;
                }
                return {c, rSq};
            }

            Graphene::Vec2f pA = (p1 + p2) / 2.0f;
            Graphene::Vec2f pB = (p1 + p3) / 2.0f;
            float delta = vA.x * vB.y - vA.y * vB.x;
            float t = ((pB.x - pA.x) * vB.y - (pB.y - pA.y) * vB.x) / delta;
            c = pA + vA * t;
            rSq = lengthSq(c - p1);
            return {c, rSq};
        }

        float radiusSq = 0.0f;

    public:
        Graphene::Vec2f center = {0.0f, 0.0f};
        float radius = 0.0f;

        SmallestEnclosingCircle() = default;

        void addPoint(Graphene::Vec2f p) {
            points.push_back(p);
        }

        void eval() {
            center = {0.0f, 0.0f};
            radiusSq = 0.0f;
            for (int i = 0; i < points.size(); i++) {
                if (outside(points[i], center, radiusSq)) {
                    center = points[i];
                    radiusSq = 0.0f;
                    for (int j = 0; j < i; j++) {
                        if (outside(points[j], center, radiusSq)) {
                            std::tie(center, radiusSq) = makeCircle(points[i], points[j]);
                            for (int k = 0; k < j; k++) {
                                if (outside(points[k], center, radiusSq))
                                    std::tie(center, radiusSq) = makeCircle(points[i], points[j], points[k]);
                            }
                        }
                    }
                }
            }
            radius = std::pow(radiusSq, 0.5f);
        }
    };

}
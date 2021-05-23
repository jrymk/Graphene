#include <vector>
#include "../graphene/Structure.hpp"

namespace Utils {

    class SmallestEnclosingCircle {
    private:
        std::vector<Graphene::Vec2f> points;

    public:
        Graphene::Vec2f center = {0.0f, 0.0f};
        float radius = 0.0f;

        SmallestEnclosingCircle() = default;

        void addPoint(Graphene::Vec2f p) {
            points.push_back(p);
        }

        void eval() {
            radius = 0.0f;
            for (int i = 0; i < points.size(); i++) {
                if ((points[i] - center).length() * (points[i] - center).length() > radius) {
                    center = points[i];
                    radius = 0.0f;
                    for (int j = 0; j < i; j++) {
                        if ((points[j] - center).length() * (points[j] - center).length() > radius) {
                            center = (points[i] + points[j]) / 2.0f;
                            radius = (points[i] + points[j]).length() / 2.0f;
                            for (int k = 0; k < j; k++) {
                                if ((points[k] - center).length() * (points[k] - center).length() > radius) {
                                    Graphene::Vec2f perpA = Graphene::Vec2f(points[i].y - points[j].y, points[j].x - points[i].x);
                                    Graphene::Vec2f perpB = Graphene::Vec2f(points[i].y - points[k].y, points[k].x - points[i].x);
                                    if ((perpA.x * perpB.y - perpA.y * perpB.x) < FLT_MIN) {
                                        Graphene::Vec2f tCenter(0.0f, 0.0f);
                                        double tRadius;

                                        center = (points[i] + points[j]) / 2.0f;
                                        radius = (points[i] + points[j]).length() / 2.0f;

                                        tCenter = (points[i] + points[k]) / 2.0f;
                                        tRadius = (points[i] + points[k]).length() / 2.0f;

                                        if (tRadius > radius) {
                                            center = tCenter;
                                            radius = tRadius;
                                        }

                                        tCenter = (points[j] + points[k]) / 2.0f;
                                        tRadius = (points[j] + points[k]).length() / 2.0f;

                                        if (tRadius > radius) {
                                            center = tCenter;
                                            radius = tRadius;
                                        }

                                        return;
                                    }

                                    Graphene::Vec2f cA = (points[i] + points[j]) / 2.0f;
                                    Graphene::Vec2f cB = (points[i] + points[k]) / 2.0f;
                                    float delta = perpA.x * perpB.y - perpA.y * perpB.x;
                                    float t = ((cB.x - cA.x) * perpB.y - (cB.y - cA.y) * perpB.x) / delta;
                                    center = cA + perpA * t;
                                    radius = (center - points[i]).length();
                                    return;
                                }
                            }
                        }
                    }
                }
            }
            radius = std::pow(radius, 0.5f);
        }
    };

}
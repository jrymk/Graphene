#include <Core/Placement/PosInitializer.h>
#include <set>

namespace gfn {

    namespace {
        const double VERTEX_CIRCLE = 1;
        const double PI = acos(-1);
    }

    double ComponentInitializer::getWidth(BiconnectedComponent *b) {
        return (b->size() + 1) / 2 * VERTEX_CIRCLE;
    }

    void ComponentInitializer::dfs1(BiconnectedComponent *now, BiconnectedComponent *parent) {
        width[now] = getWidth(now);
        double sum = 0;
        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            dfs1(i, now);
            sum += width[i];
        }
        width[now] = std::max(width[now], sum);
    }

    void ComponentInitializer::dfs2(BiconnectedComponent *now, BiconnectedComponent *parent, int d) {
        if (depth.size() <= d) depth.emplace_back();
        depth[d].emplace_back(now);
        double sum = 0;
        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            sum += width[i];
        }

        double angle = fromAngle[now];
        double nowSize = angleSize[now];
        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            double iSize = nowSize * width[i] / sum;
            fromAngle[i] = angle;
            angleSize[i] = iSize;
            angle += iSize;
            dfs2(i, now, d + 1);
        }
    }

    double ComponentInitializer::initDepth(int d, double radius) {

        if (d == 0) radius = 0;
        else {
            double lastRadius = radius;
            for (auto i : depth[d]) {
                radius = std::max(radius, getWidth(i) / angleSize[i]);
                radius = std::max(radius, lastRadius + getWidth(i) / 2);
            }
        }

        double out = radius;
        for (auto i : depth[d]) {

            double t = fromAngle[i] + angleSize[i] / 2;
            Vec2 center = Vec2(radius * std::cos(t), radius * std::sin(t));
            double r = getWidth(i) / 2;
            double single = 2 * PI / i->size();
            double now = 0;
            out = std::max(out, radius + r + VERTEX_CIRCLE);

            for (auto j : i->getVertices()) {
                position->pos[j] = center + Vec2(r * std::cos(now), r * std::sin(now));
                now += single;
            }

        }

        return out;
    }

    ComponentInitializer::ComponentInitializer(Component *c) : component(c) {
        position = new ComponentPosition({component});
    }

    void ComponentInitializer::findRoot1(gfn::BiconnectedComponent *now, gfn::BiconnectedComponent *parent) {
        subtreeDepth[now] = getWidth(now);
        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            findRoot1(i, now);
            subtreeDepth[now] = std::max(subtreeDepth[now], subtreeDepth[i] + VERTEX_CIRCLE + getWidth(now));
        }
    }

    void ComponentInitializer::findRoot2(gfn::BiconnectedComponent *now, gfn::BiconnectedComponent *parent) {
        std::multiset<double> subtrees;
        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            subtrees.insert(subtreeDepth[i]);
        }
        subtrees.insert(upDepth[now]);
        rootDepth[now] = *subtrees.rbegin() + VERTEX_CIRCLE + getWidth(now);
        if (root == nullptr || rootDepth[now] < rootDepth[root]) root = now;

        for (auto i : component->getBlockCutTree()->getAdjacentBCC(now)) {
            if (i == parent) continue;
            subtrees.erase(subtrees.find(subtreeDepth[i]));
            upDepth[i] = *subtrees.rbegin() + getWidth(now) + 2 * VERTEX_CIRCLE;
            findRoot2(i, now);
        }
    }

    ComponentPosition *ComponentInitializer::init() {

        BiconnectedComponent *temp = *component->getBlockCutTree()->getBCCs().begin();
        findRoot1(temp, temp);
        findRoot2(temp, temp);

        assert(root != nullptr);
        dfs1(root, root);
        fromAngle[root] = 0;
        angleSize[root] = 2 * PI;
        dfs2(root, root, 0);
        double now = 0;
        for (int i = 0; i < depth.size(); i++) {
            now = initDepth(i, now);
        }
        position->diameter = now * 2;

        double minX = position->pos.begin()->second.x;
        double minY = position->pos.begin()->second.y;

        for (auto i : position->pos) {
            minX = std::min(minX, i.second.x);
            minY = std::min(minY, i.second.y);
        }

        for (auto &i : position->pos) {
            i.second.x -= minX;
            i.second.y -= minY;
        }

        return position;
    }

    void PosInitializer::putComponent(ComponentPosition *c, Vec2 pos) {
        for (auto p : c->pos) {
            p.first->props->position.value = pos + p.second;
        }
    }

    PosInitializer::PosInitializer(Structure *s) : structure(s) {}

    void PosInitializer::init() {

        std::vector<ComponentPosition *> components;

        for (auto c : structure->components) {
            ComponentPosition *pos = ComponentInitializer(c).init();
            components.emplace_back(pos);
        }

        std::sort(components.begin(), components.end(), [](ComponentPosition *a, ComponentPosition *b) {
            return a->diameter > b->diameter;
        });

        double minX = 0, minY = 0, maxX = 0, maxY = 0;
        double newMinX = minX, newMinY = minY, newMaxX = maxX, newMaxY = maxY;
        int side = -1;
        Vec2 now(0, 0);
        for (auto i : components) {
            if (side == -1) {
                putComponent(i, now);
                maxX += i->diameter;
                maxY += i->diameter;
                newMaxX = maxX;
                newMaxY = maxY;
                side = 0;
                now = Vec2(minX, maxY);
                continue;
            }

            if (side == 0) {
                if (now.x + i->diameter > maxX) {
                    side = 1;
                    minX = newMinX;
                    minY = newMinY;
                    maxX = newMaxX;
                    maxY = newMaxY;
                    now = Vec2(maxX, maxY);
                }
            } else if (side == 1) {
                if (now.y - i->diameter < minY) {
                    side = 2;
                    minX = newMinX;
                    minY = newMinY;
                    maxX = newMaxX;
                    maxY = newMaxY;
                    now = Vec2(maxX, minY);
                }
            } else if (side == 2) {
                if (now.x - i->diameter < minX) {
                    side = 3;
                    minX = newMinX;
                    minY = newMinY;
                    maxX = newMaxX;
                    maxY = newMaxY;
                    now = Vec2(minX, minY);
                }
            } else {
                if (now.y + i->diameter > maxY) {
                    side = 0;
                    minX = newMinX;
                    minY = newMinY;
                    maxX = newMaxX;
                    maxY = newMaxY;
                    now = Vec2(minX, maxY);
                }
            }

            if (side == 0) {
                putComponent(i, now);
                newMaxY = std::max(newMaxY, maxY + i->diameter);
                now += Vec2(i->diameter, 0);
            } else if (side == 1) {
                putComponent(i, now + Vec2(0, -i->diameter));
                newMaxX = std::max(newMaxX, maxX + i->diameter);
                now += Vec2(0, -i->diameter);
            } else if (side == 2) {
                putComponent(i, now + Vec2(-i->diameter, -i->diameter));
                newMinY = std::min(newMinY, minY - i->diameter);
                now += Vec2(-i->diameter, 0);
            } else {
                putComponent(i, now + Vec2(-i->diameter, 0));
                newMinX = std::min(newMinX, minX - i->diameter);
                now += Vec2(0, i->diameter);
            }

        }

    }

}
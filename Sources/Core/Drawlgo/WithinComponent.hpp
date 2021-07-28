#pragma once

#include <cmath>
#include <Structure/Structure.hpp>
#include <Properties/Properties.hpp>
#include <Configs/Configs.hpp>

namespace gfn::core::drawlgo {
    double distance(gfn::structure::Vertex* u, gfn::structure::Vertex* v) {
        double dx = u->props->position.x - v->props->position.x;
        double dy = u->props->position.y - v->props->position.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    gfn::Vec2f repelForce(gfn::configs::Configs* configs, gfn::structure::Vertex* u,
                          gfn::structure::Vertex* v) {
        double dis = distance(u, v);
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = -configs->c3 / dis / dis;
        return (v->props->position - u->props->position).normalize() * coeff;
    }

    gfn::Vec2f attractForce(gfn::configs::Configs* configs, gfn::structure::Vertex* u,
                            gfn::structure::Vertex* v) {
        double dis = distance(u, v);
        if (dis == 0.0f)
            return Vec2f(0, 0);
        double coeff = configs->c1 * log(dis / configs->c2);
        return (v->props->position - u->props->position).normalize() * coeff;
    }

    void updateComponent(gfn::configs::Configs* configs, gfn::structure::Component* c,
                         gfn::properties::Properties* properties) {
        for (int i = 0; i < 1; i++) {
            for (auto& u : c->getAdjList())
                u.first->internalProps->force = gfn::Vec2f(0.0, 0.0);
            for (auto& u : c->getAdjList()) {
                for (auto& v : c->getAdjacentVertices(u.first)) {
                    u.first->internalProps->force += attractForce(configs, u.first, v);
                }
                for (auto& v : c->getAdjList()) {
                    if (u.first == v.first)
                        continue;
                    u.first->internalProps->force += repelForce(configs, u.first, v.first);
                    v.first->internalProps->force += repelForce(configs, v.first, u.first);
                }
            }
            for (auto& u : c->getAdjList())  // flush move
                u.first->props->position += u.first->internalProps->force * configs->c4;
        }
    }

}
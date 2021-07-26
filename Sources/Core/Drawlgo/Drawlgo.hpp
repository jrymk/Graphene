#pragma once

#include <cmath>
#include <Core/Structure/Structure.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Configs/Configs.hpp>

// updater is such a bad name, I need a new one
namespace gfn::core::drawlgo {
    class Drawlgo {
    public:
        double distance(gfn::core::structure::Vertex *u, gfn::core::structure::Vertex *v) {
            double dx = u->prop->position.x - v->prop->position.x;
            double dy = u->prop->position.y - v->prop->position.y;
            return std::sqrt(dx * dx + dy * dy);
        }

        gfn::core::Vec2f repelForce(gfn::core::configs::Configs *configs, gfn::core::structure::Vertex *u,
                                    gfn::core::structure::Vertex *v) {
            double dis = distance(u, v);
            if (dis == 0.0)
                return {0.0, 0.0};
            double coeff = -configs->c3 / dis / dis;
            return (v->prop->position - u->prop->position).normalize() * coeff;
        }

        gfn::core::Vec2f attractForce(gfn::core::configs::Configs *configs, gfn::core::structure::Vertex *u,
                                      gfn::core::structure::Vertex *v) {
            double dis = distance(u, v);
            if (dis == 0.0f)
                return Vec2f(0, 0);
            double coeff = configs->c1 * log(dis / configs->c2);
            return (v->prop->position - u->prop->position).normalize() * coeff;
        }

        void update(gfn::core::configs::Configs *configs, gfn::core::structure::Structure *structure,
                    gfn::core::properties::Properties *properties) {
            for (auto &c : structure->componentList.getComponents()) {
                for (auto &u : c->getAdjList())
                    u.first->prop->force = gfn::core::Vec2f(0.0, 0.0);
                for (auto &u : c->getAdjList()) {
                    for (auto &v : c->getAdjacentVertices(u.first)) {
                        u.first->prop->force += repelForce(configs, u.first, v);
                        v->prop->force += repelForce(configs, v, u.first);
                    }
                    for (auto &v : c->getAdjList()) {
                        if (u.first == v.first)
                            continue;
                        u.first->prop->force += attractForce(configs, u.first, v.first);
                        v.first->prop->force += attractForce(configs, v.first, u.first);
                    }
                }
                for (auto &u : c->getAdjList())// flush move
                    u.first->prop->position += u.first->prop->force * configs->c4;
            }
        }
    };
} // namespace gfn::core::drawlgo
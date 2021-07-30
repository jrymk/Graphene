#pragma once

#include <iomanip>
#include <cmath>
#include <Structure/Structure.hpp>
#include <Properties/Properties.hpp>
#include <Configs/Configs.hpp>

namespace gfn::core::drawlgo {
    double distance(gfn::Vec2f u, gfn::Vec2f v) {
        double dx = u.x - v.x;
        double dy = u.y - v.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    gfn::Vec2f repelForce(gfn::configs::Configs* configs, gfn::Vec2f u, gfn::Vec2f v) {
        double dis = distance(u, v);
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = -configs->c3 / dis / dis;
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2f attractForce(gfn::configs::Configs* configs, gfn::Vec2f u, gfn::Vec2f v) {
        double dis = distance(u, v);
        if (dis == 0.0f)
            return {0, 0};
        double coeff = configs->c1 * log(dis / configs->c2);
        return (v - u).normalize() * coeff;
    }

    void updateVertex(gfn::configs::Configs* configs, gfn::structure::Component* c, gfn::structure::Vertex* u) {
        for (auto& v : c->getAdjacentVertices(u))
            u->internalProps->force += attractForce(configs, u->props->position, v->props->position);
        for (auto& v : c->vertices) {
            if (u == v)
                continue;
            u->internalProps->force += repelForce(configs, u->props->position, v->props->position);
            //v->internalProps->force += repelForce(configs, v->props->position, u->props->position);
        }
        /*for (auto& e : c->edges) {
            u->internalProps->force += repelForce(configs, u->props->position, e->props->position);
            //e->internalProps->force += repelForce(configs, e->props->position, u->props->position);
        }*/
    }

    /*void updateEdge(gfn::configs::Configs* configs, gfn::structure::Component* c, gfn::structure::Edge* u) {
        u->internalProps->force += attractForce(configs, u->props->position, u->startVertex->props->position);
        u->internalProps->force += attractForce(configs, u->props->position, u->endVertex->props->position);
        u->startVertex->internalProps->force += attractForce(configs, u->startVertex->props->position,
                                                             u->props->position);
        u->endVertex->internalProps->force += attractForce(configs, u->endVertex->props->position,
                                                             u->props->position);

        for (auto& v : c->vertices) {
            u->internalProps->force += repelForce(configs, u->props->position, v->props->position);
            //v->internalProps->force += repelForce(configs, v->props->position, u->props->position);
        }
        for (auto& e : c->edges) {
            if (e == u)
                continue;
            u->internalProps->force += repelForce(configs, u->props->position, e->props->position);
            //e->internalProps->force += repelForce(configs, e->props->position, u->props->position);
        }
    }*/

    void updateComponentSingleThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        for (auto& u : c->vertices)
            u->internalProps->force = gfn::Vec2f(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->internalProps->force = gfn::Vec2f(0.0, 0.0);*/

        for (auto& u : c->vertices)
            updateVertex(configs, c, u);
        /*for (auto& e : c->edges)
            updateEdge(configs, c, e);*/

        // flush move
        for (auto& u : c->vertices)
            u->props->position += u->internalProps->force * configs->c4;
        /*for (auto& e : c->edges)
            e->props->position += e->internalProps->force * configs->c4;*/
    }

    void updateComponentMultiThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        for (auto& u : c->vertices)
            u->internalProps->force = gfn::Vec2f(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->internalProps->force = gfn::Vec2f(0.0, 0.0);*/

        thread_pool::ThreadPool thread_pool{};
        std::vector<std::future<void>> futures;
        for (auto& u : c->vertices)
            futures.emplace_back(thread_pool.Submit(updateVertex, configs, c, u));
        /* for (auto& e : c->edges)
             futures.emplace_back(thread_pool.Submit(updateEdge, configs, c, e));*/
        for (auto& it : futures)
            it.wait();

        for (auto& u : c->vertices)
            u->props->position += u->internalProps->force * configs->c4;
        /*for (auto& e : c->edges)
            e->props->position += e->internalProps->force * configs->c4;*/
    }
}
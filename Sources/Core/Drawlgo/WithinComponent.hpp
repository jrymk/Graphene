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
        double coeff = -configs->c3.value / dis / dis;
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2f attractForce(gfn::configs::Configs* configs, gfn::Vec2f u, gfn::Vec2f v) {
        double dis = distance(u, v);
        if (dis == 0.0f)
            return {0, 0};
        double coeff = configs->c1.value * log(dis / configs->c2.value);
        return (v - u).normalize() * coeff;
    }

    void updateVertex(gfn::configs::Configs* configs, gfn::structure::Component* c, gfn::structure::Vertex* u) {
        for (auto& v : c->getAdjacentVertices(u))
            u->props->force.value += attractForce(configs, u->props->position.value, v->props->position.value);
        for (auto& v : c->vertices) {
            if (u == v)
                continue;
            u->props->force.value += repelForce(configs, u->props->position.value, v->props->position.value);
            //v->props->force += repelForce(configs, v->props->position, u->props->position);
        }
        /*for (auto& e : c->edges) {
            u->props->force += repelForce(configs, u->props->position, e->props->position);
            //e->props->force += repelForce(configs, e->props->position, u->props->position);
        }*/
    }

    /*void updateEdge(gfn::configs::Configs* configs, gfn::structure::Component* c, gfn::structure::Edge* u) {
        u->props->force += attractForce(configs, u->props->position, u->startVertex->props->position);
        u->props->force += attractForce(configs, u->props->position, u->endVertex->props->position);
        u->startVertex->props->force += attractForce(configs, u->startVertex->props->position,
                                                             u->props->position);
        u->endVertex->props->force += attractForce(configs, u->endVertex->props->position,
                                                             u->props->position);

        for (auto& v : c->vertices) {
            u->props->force += repelForce(configs, u->props->position, v->props->position);
            //v->props->force += repelForce(configs, v->props->position, u->props->position);
        }
        for (auto& e : c->edges) {
            if (e == u)
                continue;
            u->props->force += repelForce(configs, u->props->position, e->props->position);
            //e->props->force += repelForce(configs, e->props->position, u->props->position);
        }
    }*/

    unsigned long long updateComponentSingleThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        gfn::timer::Timer timer;
        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2f(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->props->force = gfn::Vec2f(0.0, 0.0);*/

        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                updateVertex(configs, c, u);
        }
        /*for (auto& e : c->edges)
            updateEdge(configs, c, e);*/

        // flush move
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * configs->c4.value;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * configs->c4;*/

        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
        }

        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Single: that took " << t << "us\n";
        return timer.getMicroseconds();
    }

    unsigned long long updateComponentMultiThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        gfn::timer::Timer timer;
        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2f(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->props->force = gfn::Vec2f(0.0, 0.0);*/

        thread_pool::ThreadPool thread_pool{};
        std::vector<std::future<void>> futures;
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                futures.emplace_back(thread_pool.Submit(updateVertex, configs, c, u));
        }
        /* for (auto& e : c->edges)
         futures.emplace_back(thread_pool.Submit(updateEdge, configs, c, e));*/
        for (auto& it : futures)
            it.wait();

        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * configs->c4.value;
        }
        
        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * configs->c4;*/
        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Multi: that took " << t << "us\n";
        return timer.getMicroseconds();
    }
}
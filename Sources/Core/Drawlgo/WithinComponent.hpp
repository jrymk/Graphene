#pragma once

#include <iomanip>
#include <cmath>
#include <Interface/Interface.hpp>
#include <Structure/Structure.h>
#include <Properties/Properties.h>
#include <Configs/Configs.hpp>
#include <minitrace.h>
#include <System/Timer.hpp>

namespace gfn::core::drawlgo {
    double distance(gfn::Vec2 u, gfn::Vec2 v) {
        double dx = u.x - v.x;
        double dy = u.y - v.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    gfn::Vec2 repelForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = distance(u, v);
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = -itf->cfg.getWrite().c3.value / dis / dis;
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 attractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = distance(u, v);
        if (dis == 0.0f)
            return {0, 0};
        double coeff = itf->cfg.getWrite().c1.value * log(dis / itf->cfg.getWrite().c2.value);
        return (v - u).normalize() * coeff;
    }

    void updateVertex(gfn::Interface* itf, gfn::Component* c, gfn::Vertex* u) {
        MTR_SCOPE("core", "drawlgo update vertex");

        for (auto& v : c->getAdjacentVertices(u))
            u->props->force.value += attractForce(itf, u->props->position.value, v->props->position.value);
        for (auto& v : c->vertices) {
            if (u == v)
                continue;
            u->props->force.value += repelForce(itf, u->props->position.value, v->props->position.value);
            //v->props->force += repelForce(itf->cfg.getWrite().v->props->position, u->props->position);
        }
        /*for (auto& e : c->edges) {
            u->props->force += repelForce(itf->cfg.getWrite().u->props->position, e->props->position);
            //e->props->force += repelForce(itf->cfg.getWrite().e->props->position, u->props->position);
        }*/
    }

    /*void updateEdge(gfn::Interface* itf, gfn::structure::Component* c, gfn::structure::Edge* u) {
        u->props->force += attractForce(itf->cfg.getWrite().u->props->position, u->startVertex->props->position);
        u->props->force += attractForce(itf->cfg.getWrite().u->props->position, u->endVertex->props->position);
        u->startVertex->props->force += attractForce(itf->cfg.getWrite().u->startVertex->props->position,
                                                             u->props->position);
        u->endVertex->props->force += attractForce(itf->cfg.getWrite().u->endVertex->props->position,
                                                             u->props->position);

        for (auto& v : c->vertices) {
            u->props->force += repelForce(itf->cfg.getWrite().u->props->position, v->props->position);
            //v->props->force += repelForce(itf->cfg.getWrite().v->props->position, u->props->position);
        }
        for (auto& e : c->edges) {
            if (e == u)
                continue;
            u->props->force += repelForce(itf->cfg.getWrite().u->props->position, e->props->position);
            //e->props->force += repelForce(itf->cfg.getWrite().e->props->position, u->props->position);
        }
    }*/

    unsigned long long updateComponentSingleThreaded(gfn::Interface* itf, gfn::Component* c) {
        MTR_SCOPE("core", "drawlgo update single thread");
        gfn::Timer timer;
        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->props->force = gfn::Vec2f(0.0, 0.0);*/

        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                updateVertex(itf, c, u);
        }
        /*for (auto& e : c->edges)
            updateEdge(itf->cfg.getWrite().c, e);*/

        // flush move
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * itf->cfg.getWrite().c4.value;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * itf->cfg.getWrite().c4;*/

        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
        }

        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Single: that took " << t << "us\n";
        return timer.getMicroseconds();
    }

    unsigned long long updateComponentMultiThreaded(gfn::Interface* itf, gfn::Component* c) {
        MTR_META_PROCESS_NAME("graphene-core");
        MTR_META_THREAD_NAME("drawlgo worker");
        MTR_SCOPE("core", "drawlgo update multi thread");

        gfn::Timer timer;
        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->props->force = gfn::Vec2f(0.0, 0.0);*/

        thread_pool::ThreadPool thread_pool{};
        std::vector<std::future<void>> futures;
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                futures.emplace_back(thread_pool.Submit(updateVertex, itf, c, u));
        }
        /* for (auto& e : c->edges)
         futures.emplace_back(thread_pool.Submit(updateEdge, itf->cfg.getWrite().c, e));*/
        {
            MTR_SCOPE("core", "graph update wait");
            for (auto& it : futures)
                it.wait();
        }

        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * itf->cfg.getWrite().c4.value;
        }

        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
            /// TODO
            e->props->position.get() = (e->props->startVertexPosition.get() + e->props->endVertexPosition.get()) / 2.0;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * itf->cfg.getWrite().c4;*/
        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Multi: that took " << t << "us\n";
        return timer.getMicroseconds();
    }
}
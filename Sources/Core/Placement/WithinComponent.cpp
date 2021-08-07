#include "Placement.h"
#include <thread_pool.hpp>
#include <Tracy.hpp>

namespace gfn {
    gfn::Vec2 Placement::repelForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = -itf->graph.getWrite().cfg.c3.value / dis / dis;
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 Placement::attractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0f)
            return {0, 0};
        double coeff = itf->graph.getWrite().cfg.c1.value * log(dis / itf->graph.getWrite().cfg.c2.value);
        return (v - u).normalize() * coeff;
    }

    void Placement::updateVertex(gfn::Interface* itf, gfn::Component* c, gfn::Vertex* u) {
        ZoneScoped

        for (auto& v : c->getAdjacentVertices(u))
            u->props->force.value += attractForce(itf, u->props->position.value, v->props->position.value);
        for (auto& v : c->vertices) {
            if (u == v)
                continue;
            u->props->force.value += repelForce(itf, u->props->position.value, v->props->position.value);
            //v->props->force += repelForce(itf->graph.getWrite().cfg.v->props->position, u->props->position);
        }
        /*for (auto& e : c->edges) {
            u->props->force += repelForce(itf->graph.getWrite().cfg.u->props->position, e->props->position);
            //e->props->force += repelForce(itf->graph.getWrite().cfg.e->props->position, u->props->position);
        }*/
    }

    /*void updateEdge(gfn::Interface* itf, gfn::structure::Component* c, gfn::structure::Edge* u) {
        u->props->force += attractForce(itf->graph.getWrite().cfg.u->props->position, u->startVertex->props->position);
        u->props->force += attractForce(itf->graph.getWrite().cfg.u->props->position, u->endVertex->props->position);
        u->startVertex->props->force += attractForce(itf->graph.getWrite().cfg.u->startVertex->props->position,
                                                             u->props->position);
        u->endVertex->props->force += attractForce(itf->graph.getWrite().cfg.u->endVertex->props->position,
                                                             u->props->position);

        for (auto& v : c->vertices) {
            u->props->force += repelForce(itf->graph.getWrite().cfg.u->props->position, v->props->position);
            //v->props->force += repelForce(itf->graph.getWrite().cfg.v->props->position, u->props->position);
        }
        for (auto& e : c->edges) {
            if (e == u)
                continue;
            u->props->force += repelForce(itf->graph.getWrite().cfg.u->props->position, e->props->position);
            //e->props->force += repelForce(itf->graph.getWrite().cfg.e->props->position, u->props->position);
        }
    }*/

    void Placement::updateComponentSingleThreaded(gfn::Interface* itf, gfn::Component* c) {
        ZoneScoped

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
            updateEdge(itf->graph.getWrite().cfg.c, e);*/

        // flush move
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * itf->graph.getWrite().cfg.c4.value;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * itf->graph.getWrite().cfg.c4;*/

        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
        }

        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Single: that took " << t << "us\n";
    }

    void Placement::updateComponentMultiThreaded(gfn::Interface* itf, gfn::Component* c) {
        ZoneScoped

        gfn::Timer timer;
        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2(0.0, 0.0);
        /*for (auto& e : c->edges)
            e->props->force = gfn::Vec2f(0.0, 0.0);*/
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                pool.push_task(&Placement::updateVertex, itf, c, u);
        }
        /* for (auto& e : c->edges)
         futures.emplace_back(thread_pool.Submit(updateEdge, itf->graph.getWrite().cfg.c, e));*/
        pool.wait_for_tasks();
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                u->props->position.value += u->props->force.value * itf->graph.getWrite().cfg.c4.value;
        }

        for (auto& e : c->edges) {
            e->props->startVertexPosition = e->startVertex->props->position;
            e->props->endVertexPosition = e->endVertex->props->position;
            /// TODO
            e->props->position.get() = (e->props->startVertexPosition.get() + e->props->endVertexPosition.get()) / 2.0;
        }
        /*for (auto& e : c->edges)
            e->props->position += e->props->force * itf->graph.getWrite().cfg.c4;*/
        unsigned long long t = timer.getMicroseconds();
        //std::cerr << "Multi: that took " << t << "us\n";
    }
}
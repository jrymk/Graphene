#include "Placement.h"

namespace gfn {
    gfn::Vec2 Placement::attractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0f)
            return {0, 0};
        double coeff = itf->graph.getWrite().cfg.c1.value * log(dis / itf->graph.getWrite().cfg.c2.value);
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 Placement::repelForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = std::max(-itf->graph.getWrite().cfg.c3.value / dis / dis, -100.0);
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 Placement::edgeAttractForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0f)
            return {0, 0};
        double coeff = itf->graph.getWrite().cfg.c5.value * log(dis / itf->graph.getWrite().cfg.c6.value);
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 Placement::edgeRepelEdgeForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = std::max(-itf->graph.getWrite().cfg.c7.value / dis / dis / dis, -100.0);
        return (v - u).normalize() * coeff;
    }

    gfn::Vec2 Placement::edgeRepelVertexForce(gfn::Interface* itf, gfn::Vec2 u, gfn::Vec2 v) {
        double dis = (u - v).length();
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = std::max(-itf->graph.getWrite().cfg.c8.value / dis / dis / dis, -100.0);
        return (v - u).normalize() * coeff;
    }

    void Placement::updateVertex(gfn::Interface* itf, gfn::Component* c, gfn::Vertex* u) {
        //u->props->force.value.y -= 0.001;

        for (auto& v : c->getAdjacentVertices(u)) {
            u->props->force.value += attractForce(itf, u->props->position.value, v->props->position.value);

            //v->props->force += repelForce(itf->graph.getWrite().cfg.v->props->position, u->props->position);
            /*std::vector<Edge*> edges;
            auto edgesUV = c->getAdjList().find(u)->second.second.find(v);
            if (edgesUV != c->getAdjList().find(u)->second.second.end()) {
                for (auto& e : edgesUV->second)
                    edges.push_back(e);
            }
            auto edgesVU = c->getAdjList().find(v)->second.second.find(u);
            if (edgesVU != c->getAdjList().find(u)->second.second.end()) {
                for (auto& e : edgesVU->second)
                    edges.push_back(e);
            }*/
        }
        for (auto& v : c->vertices) {
            if (u == v)
                continue;
            //if (c->getAdjacentVertices(u).find(v) == c->getAdjacentVertices(u).end())
            //    continue;
            u->props->force.value += repelForce(itf, u->props->position.value, v->props->position.value);
        }
        /*for (auto& e : c->edges) {
            u->props->force += repelForce(itf->graph.getWrite().cfg.u->props->position, e->props->position);
            //e->props->force += repelForce(itf->graph.getWrite().cfg.e->props->position, u->props->position);
        }*/
    }

    void Placement::updateEdge(gfn::Interface* itf, gfn::Component* c, gfn::Edge* e) {
        if (c->getAdjList().find(e->startVertex)->second.second.find(e->endVertex) != c->getAdjList().find(e->startVertex)->second.second.end()) {
            for (auto& f : c->getAdjList().find(e->startVertex)->second.second.find(e->endVertex)->second) {
                if (e == f)
                    continue;
                e->props->force.value += edgeRepelEdgeForce(itf, e->props->position.value, f->props->position.value);
            }
        }
        if (c->getAdjList().find(e->endVertex)->second.second.find(e->startVertex) != c->getAdjList().find(e->endVertex)->second.second.end()) {
            for (auto& f : c->getAdjList().find(e->endVertex)->second.second.find(e->startVertex)->second) {
                if (e == f)
                    continue;
                e->props->force.value += edgeRepelEdgeForce(itf, e->props->position.value, f->props->position.value);
            }
        }
        e->props->force.value += edgeAttractForce(itf, e->props->position.value, e->startVertex->props->position.value);
        e->props->force.value += edgeAttractForce(itf, e->props->position.value, e->endVertex->props->position.value);

        for (auto& v : c->vertices) {
            e->props->force.value += edgeRepelVertexForce(itf, e->props->position.value, v->props->position.value);
        }
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
        stablized = true;

        for (auto& u : c->vertices)
            u->props->force.value = gfn::Vec2(0.0, 0.0);
        for (auto& e : c->edges)
            e->props->force.value = gfn::Vec2(0.0, 0.0);

        std::vector<std::future<void> > results;
        for (auto& u : c->vertices) {
            if (!u->props->pauseUpdate.value)
                results.emplace_back(pool.enqueue(&Placement::updateVertex, itf, c, u));
        }

        if (itf->graph.getWrite().cfg.qualityMode) {
            for (auto& e : c->edges) {
                if (!e->props->pauseUpdate.value)
                    results.emplace_back(pool.enqueue(&Placement::updateEdge, itf, c, e));
            }
        }

        for (auto& f : results)
            f.get();

        for (auto& e : c->edges) {
            e->props->startVertexPosition.value = e->startVertex->props->position.value;
            e->props->endVertexPosition.value = e->endVertex->props->position.value;

            if (!itf->graph.getWrite().cfg.doGraphUpdate.value)
                e->props->position.value = (e->props->startVertexPosition.value + e->props->endVertexPosition.value) / 2.0;
        }

        if (itf->graph.getWrite().cfg.doGraphUpdate.value) {
            for (auto& u : c->vertices) {
                if (!u->props->pauseUpdate.value)
                    u->props->position.value += u->props->force.value * itf->graph.getWrite().cfg.c4.value;

                if ((u->props->force.value * itf->graph.getWrite().cfg.c4.value).length() >= 0.000000001)
                    stablized = false;
            }

            for (auto& e : c->edges) {
                e->props->startVertexPosition.value = e->startVertex->props->position.value;
                e->props->endVertexPosition.value = e->endVertex->props->position.value;

                if (!e->props->pauseUpdate.value)
                    e->props->position.value += e->props->force.value * itf->graph.getWrite().cfg.c9.value;
                e->props->position.value = (e->props->startVertexPosition.value + e->props->endVertexPosition.value) / 2.0;

                auto ep = e->props->position.value;
                auto up = e->props->startVertexPosition.value;
                auto vp = e->props->endVertexPosition.value;
                auto perp = (vp - up).rotate(M_PI_2);
                auto mid = (vp + up) / 2.0f;

                if (e->props->startVertexUuid.value != e->props->endVertexUuid.value)
                    e->props->position.value = mid + perp.normalize() * (((ep - mid) * perp) / perp.length()); // mid + )

                if ((e->props->force.value * itf->graph.getWrite().cfg.c9.value).length() >= 0.000000002)
                    stablized = false;
            }
        }

        if (!stablized)
            itf->graph.getWrite().cfg.energySavingMode = false;

        if (itf->graph.getWrite().cfg.qualityMode) {
            for (auto& e : c->edges)
                e->props->position.value += e->props->force.value * itf->graph.getWrite().cfg.c9.value;
        }
        //std::cerr << "Multi: that took " << t << "us\n";
    }
}
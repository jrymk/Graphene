#pragma once

#include <iomanip>
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

    gfn::Vec2f repelForce(gfn::configs::Configs* configs, gfn::structure::Vertex* u, gfn::structure::Vertex* v) {
        double dis = distance(u, v);
        if (dis == 0.0)
            return {0.0, 0.0};
        double coeff = -configs->c3 / dis / dis;
        return (v->props->position - u->props->position).normalize() * coeff;
    }

    gfn::Vec2f attractForce(gfn::configs::Configs* configs, gfn::structure::Vertex* u, gfn::structure::Vertex* v) {
        double dis = distance(u, v);
        if (dis == 0.0f)
            return Vec2f(0, 0);
        double coeff = configs->c1 * log(dis / configs->c2);
        return (v->props->position - u->props->position).normalize() * coeff;
    }

    void updateVertex(gfn::configs::Configs* configs, gfn::structure::Component* c, gfn::structure::Vertex* u) {
        for (auto& v : c->getAdjacentVertices(u)) {
            u->internalProps->force += attractForce(configs, u, v);
        }
        for (auto& v : c->getAdjList()) {
            if (u == v.first)
                continue;
            u->internalProps->force += repelForce(configs, u, v.first);
            v.first->internalProps->force += repelForce(configs, v.first, u);
        }
        //std::cerr << end - start << "ns\n";
    }

    unsigned long long updateComponentSingleThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        gfn::timer::Timer timer;
        for (auto& u : c->getAdjList())
            updateVertex(configs, c, u.first);
        return timer.getMicroseconds();
    }

    unsigned long long updateComponentMultiThreaded(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        gfn::timer::Timer timer;
        thread_pool::ThreadPool thread_pool{};
        std::vector<std::future<void>> futures;
        for (auto& u : c->getAdjList())
            futures.emplace_back(thread_pool.Submit(updateVertex, configs, c, u.first));
        for (auto& it : futures)
            it.wait();
        return timer.getMicroseconds();
    }

    void updateComponent(gfn::configs::Configs* configs, gfn::structure::Component* c) {
        for (auto& u : c->getAdjList())
            u.first->internalProps->force = gfn::Vec2f(0.0, 0.0);

        updateComponentSingleThreaded(configs, c);

        for (auto& u : c->getAdjList())  // flush move
            u.first->props->position += u.first->internalProps->force * configs->c4;
    }

}
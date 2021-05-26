#pragma once

#include <windows.h>
#include <cmath>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include <unordered_map>
#include <mutex>
#include "Graph.hpp"
#include "Structure.hpp"
#include "GraphIter.hpp"
#include "../utils/ProfilerUtils.hpp"
#include "../utils/Log.hpp"
#include "../utils/SmallestEnclosingCircle.hpp"
#include "ConnectedComponent.hpp"
#include "BlockCutTree.hpp"
#include "BlockCutTreeBuilder.hpp"

namespace Graphene {

    class Core {
    private:

    public:
        // graph update rate
        Utils::FramerateCounter updateRateCounter;

        Graph* graph;

        // binds a graph to a core
        explicit Core(Graph &g) {
            graph = &g;
        }

        // force calculation constants
        float c1 = 2;
        float c2 = 1;
        float c3 = 1;
        float c4 = 0.1;

        // graph updated via visual tool
        bool pendingInputUpdate = false;

        float distance(Vertex* u, Vertex* v) {
            float dx = u->getCoord().x - v->getCoord().x;
            float dy = u->getCoord().y - v->getCoord().y;
            return sqrt(dx * dx + dy * dy);
        }

        float length(Vec2f a) {
            return sqrt(a.x * a.x + a.y * a.y);
        }

        Vec2f attractForce(Vertex* u, Vertex* v) {
            float dis = distance(u, v);
            if (dis == 0.0f)
                return Vec2f(0, 0);
            float coeff = c1 * log(dis / c2);
            return (v->getCoord() - u->getCoord()).normalize() * coeff;
        }

        Vec2f repelForce(Vertex* u, Vertex* v) {
            float dis = distance(u, v);
            if (dis == 0.0)
                return Vec2f(0, 0);
            float coeff = -c3 / dis / dis;
            return (v->getCoord() - u->getCoord()).normalize() * coeff;
        }

        Vec2f originAttractForce(Vertex* v) {
            float dis = v->getCoord().length();
            if (dis == 0.0) {
                return Vec2f(0, 0);
            }
            float coeff = std::min(1.0f, dis);
            return (Vec2f(0, 0) - v->getCoord()).normalize() * coeff;
        }

        void updatePos() {
            graph->mutex.lock();
            //NOTE: You can not run graph->updateConnectedComponent() here as this thread is different from everything else
            for (auto &component : graph->components) {
                BlockCutTreeBuilder builder(component);
                builder.build();

                ComponentVertexIter uIt(component);
                while (uIt.next()) {
                    ComponentVertexIter vIt(component);
                    while (vIt.next()) {
                        uIt.v->move(repelForce(uIt.v, vIt.v));
                        vIt.v->move(repelForce(vIt.v, uIt.v));
                    }
                }

                {
                    ComponentEdgeIter it(component);
                    while (it.next())
                        it.u->move(attractForce(it.u, it.v));
                }

                {
                    ComponentVertexIter it(component);
                    while (it.next()) {
                        if (!it.v->pauseMove)
                            it.v->flushMove(c4);
                    }
                }

                {
                    Utils::SmallestEnclosingCircle sec;
                    bool first = true;
                    ComponentVertexIter it(component);
                    while (it.next()) {
                        sec.addPoint(it.v->getCoord());

                        if (first) {
                            first = false;
                            component->bbBack = {it.v->getCoord(), it.v->getCoord()};
                        } else {
                            component->bbBack = {{std::min(it.v->getCoord().x, component->bbBack.first.x),
                                                         std::min(it.v->getCoord().y, component->bbBack.first.y)},
                                                 {std::max(it.v->getCoord().x, component->bbBack.second.x),
                                                         std::max(it.v->getCoord().y, component->bbBack.second.y)}};
                        }
                    }
                    component->bb = component->bbBack;

                    sec.eval();
                    component->center = sec.center;
                    component->radius = sec.radius;
                }
            }
            updateRateCounter.countFrame();
            graph->mutex.unlock();
        }

    };

}
#pragma once

#include <windows.h>
#include <cmath>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include <mutex>
#include <unordered_map>

#include "Graph.hpp"
#include "Structure.hpp"
#include "GraphIter.hpp"
#include "Constants.hpp"
#include "ConnectedComponent.hpp"
#include "BlockCutTree.hpp"
#include "BlockCutTreeBuilder.hpp"

namespace gph {

    class Core {
    private:
        Graph *graph = new Graph();

    public:
        // graph update rate
        Utils::CycleMonitor updateRateCounter;

        Graph *getGraphObj() {
            return graph;
        }

        std::unordered_map<Vertex *, std::unordered_multimap<Vertex *, std::unordered_set<Edge *>>> &getGraph() {
            return graph->graph;
        }

        std::unordered_set<ConnectedComponent *> getComponents() {
            return graph->components;
        }

        // binds a graph to a core
        explicit Core() {

        }

        std::mutex mutex;

        // graph updated via visual tool
        bool pendingInputUpdate = false;

        float distance(Vertex *u, Vertex *v) {
            float dx = u->getCoord().x - v->getCoord().x;
            float dy = u->getCoord().y - v->getCoord().y;
            return sqrt(dx * dx + dy * dy);
        }

        float length(Vec2f a) {
            return sqrt(a.x * a.x + a.y * a.y);
        }

        Vec2f attractForce(Vertex *u, Vertex *v) {
            double dis = Vec2f::distance(u->getCoord(), v->getCoord());
            if (dis == 0.0f)
                return Vec2f(0, 0);
            float coeff = Constants::c1 * log(dis / Constants::c2);
            return (v->getCoord() - u->getCoord()).normalize() * coeff;
        }

        Vec2f repelForce(Vertex *u, Vertex *v) {
            float dis = distance(u, v);
            if (dis == 0.0)
                return Vec2f(0, 0);
            float coeff = -Constants::c3 / dis / dis;
            return (v->getCoord() - u->getCoord()).normalize() * coeff;
        }

        Vec2f originAttractForce(Vertex *v) {
            float dis = v->getCoord().length();
            if (dis == 0.0) {
                return Vec2f(0, 0);
            }
            float coeff = std::min(Constants::c6, dis);
            return (Vec2f(0, 0) - v->getCoord()).normalize() * coeff;
        }

        void updatePosWithinComponent(ConnectedComponent *component) {
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
                        it.v->flushMove(Constants::c4);
                }
            }
        }

        void updatePosBetweenComponents() {

            for (auto component : graph->components) {
                component->updateCentroid();
            }

            VertexIter uIt(graph);
            while (uIt.next()) {
                Vertex *u = uIt.v;
                VertexIter vIt(graph);
                while (vIt.next()) {
                    Vertex *v = vIt.v;
                    if (u->component == v->component) continue;

                    Vec2f f = repelForce(u, v);
                    u->component->move(u, f);
                }
                u->component->move(nullptr, originAttractForce(u));
            }

            for (auto component : graph->components) {
                component->flushMove();
            }

        }

        void updatePos() {
            Utils::Timer timer;

            //graph->mutex.lock();
            //NOTE: You can not run graph->updateConnectedComponent() here as this thread is different from everything else
            for (auto &component : graph->components) {
                updatePosWithinComponent(component);
            }

            if (Constants::forceBetweenComponents)
                updatePosBetweenComponents();

            for (auto &component : graph->components) {

                {
                    bool first = true;
                    ComponentVertexIter it(component);
                    while (it.next()) {
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
                }
            }
            updateRateCounter.resetCycle();
            //graph->mutex.unlock();
            //LOG_VERBOSE("positions updated (took " + std::to_string(timer.getMicroseconds()) + "us)");
        }

    };

}
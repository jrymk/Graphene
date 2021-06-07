#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Structure.hpp"
#include "ConnectedComponent.hpp"
#include "BlockCutTreeBuilder.hpp"
#include "../utils/Log.hpp"
#include "../utils/ProfilerUtils.hpp"

namespace Graphene {

    class Graph {
    private:

    public:
        //                 u                                v                           edge properties
        std::unordered_map<Vertex*, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>> graph;
        // keep v removed if no edges

        Graph() = default;

        std::unordered_set<ConnectedComponent*> components;


        Vertex* debugVertexHighlight = nullptr;

        ConnectedComponent* newConnectedComponent(Vertex* root) {
            auto c = new ConnectedComponent(root);
            c->adjList.insert({root, std::unordered_set<Vertex*>()});
            components.insert(c);
            root->component = c;
            return c;
        }

        void updateConnectedComponent() {
            Utils::Timer timer;
            LOG_INFO("update components");

            for (auto &it : components) {
                if (it == nullptr)
                    LOG_WARNING("WHOA THERE! found null component");
            }

            // builds adjacency list for component update functions
            std::unordered_map<Vertex*, std::unordered_set<Vertex*>> adjList;
            std::unordered_map<Vertex*, bool> visited;

            for (auto &vIt : graph) {
                visited.insert({vIt.first, false});
                adjList.insert({vIt.first, std::unordered_set<Vertex*>()});
            }

            LOG_VERBOSE("building adjacency list");
            for (auto &uIt : graph) {
                for (auto &vIt : uIt.second) {
                    adjList.find(uIt.first)->second.insert(vIt.first);
                    adjList.find(vIt.first)->second.insert(uIt.first);
                }
            }

            LOG_VERBOSE("updating existing components");
            for (auto &it : components)
                it->updateConnectedComponent(adjList, visited);

            LOG_VERBOSE("checking for invalid components");
            for (auto component = components.begin(); component != components.end();) {
                if (!(*component)->isValidComponent()) {
                    LOG_VERBOSE((*component)->getUUID() + " is no longer a valid component");
                    component = deleteConnectedComponent((*component), true);
                } else
                    component++;
            }

            LOG_VERBOSE("checking for dangling vertices");
            for (auto &it : visited) {
                //std::cerr << it.first->UUID << " is " << it.second << " visited" << "\n";
                if (!it.second) {
                    LOG_VERBOSE(it.first->UUID + " has no component connected");
                    //std::cerr << "new component for vertex above" << "\n";
                    auto c = newConnectedComponent(it.first);
                    c->updateConnectedComponent(adjList, visited);
                }
            }

            LOG_VERBOSE("checking for block cut tree updates");
            for (auto &it : components) {
                if (it->pendingBlockCutTreeRebuild) { // Please, only rebuild when necessary
                    BlockCutTreeBuilder builder(it);
                    builder.build();
                    it->pendingBlockCutTreeRebuild = false;
                }
            }

            LOG_DEBUG("end update components (took " + std::to_string(timer.getMicroseconds()) + "us)");
        }

        std::unordered_set<ConnectedComponent*>::iterator deleteConnectedComponent(ConnectedComponent* component, bool deleteContent) {
            LOG_DEBUG("delete component " + component->getUUID() + (deleteContent ? " with " : " without ") + "deleting content");

            LOG_VERBOSE("erasing all edges");
            if (!deleteContent) {
                for (auto &uIt : component->adjList) {
                    for (auto &vIt : component->adjList) {
                        graph.find(uIt.first)->second.erase(vIt.first);
                        uIt.second.erase(vIt.first);
                    }
                }
            }

            for (auto &it : component->adjList) {
                if (deleteContent) {
                    LOG_VERBOSE("deleting vertex " + it.first->UUID);
                    graph.erase(it.first);
                    delete it.first;
                } else {
                    LOG_VERBOSE("new component for vertex " + it.first->UUID);
                    newConnectedComponent(it.first);
                }
            }

            LOG_VERBOSE("removing component " + component->getUUID() + " from list");
            auto it = components.find(component);
            delete *it;
            auto eraseIt = components.erase(it);

            LOG_VERBOSE("deleted component");
            return eraseIt;
        }

        unsigned int getVertexCount() const {
            return graph.size();
        }

        Vertex* newVertex() {
            LOG_DEBUG("new vertex");
            auto v = new Vertex(0);
            graph.insert({v, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>()});
            auto c = newConnectedComponent(v);
            BlockCutTreeBuilder builder(c);
            builder.build();
            c->pendingBlockCutTreeRebuild = false;
            return v;
        }

        void deleteEdge(Vertex* u, Vertex* v) {
            LOG_DEBUG("delete all edges connecting " + u->UUID + " and " + v->UUID);
            graph.find(u)->second.erase(v);
            graph.find(v)->second.erase(u);
            //std::cerr << "update called from deleteEdge\n";
            updateConnectedComponent();
            u->component->pendingBlockCutTreeRebuild = true;
            v->component->pendingBlockCutTreeRebuild = true;
        }

        void deleteEdgeNoUpdate(Vertex* u, Vertex* v) {
            LOG_DEBUG("delete all edges connecting " + u->UUID + " and " + v->UUID);
            graph.find(u)->second.erase(v);
            graph.find(v)->second.erase(u);
            //std::cerr << "update called from deleteEdge\n";
        }

        void deleteVertex(Vertex* v) {
            LOG_DEBUG("delete vertex " + v->UUID);
            if (v->component->getRootVertex() == v) {
                if (v->component->adjList.size() == 1) { // v is the only vertex in the component: delete the component (it should not have adjacent edges too)
                    LOG_VERBOSE("removing component " + v->component->getUUID());
                    components.erase(v->component);
                    delete v->component;
                } else { // transfer component ownership to random remaining vertex
                    LOG_VERBOSE("erasing adjacent edges with " + v->UUID);
                    for (auto &it : v->component->adjList) {
                        deleteEdgeNoUpdate(it.first, v);
                        deleteEdgeNoUpdate(v, it.first);
                    }
                    v->component->adjList.erase(v);
                    if (v == v->component->getRootVertex()) {
                        v->component->setRootVertex(v->component->adjList.begin()->first);
                        LOG_VERBOSE("transfer component " + v->component->getUUID() + " root to " + v->component->getRootVertex()->UUID);
                    }
                }
            }
            graph.erase(v);
            delete v;
            //std::cerr << "update called from deleteVertex\n";
            updateConnectedComponent();
            //LOG_DEBUG("finished delete vertex");
        }

        void resizeVertices(int count) {
            LOG_DEBUG("resize vertices to " + std::to_string(count));
            //std::cerr << "update called from resizeVertices\n";
            updateConnectedComponent();
            while (graph.size() > count)
                deleteVertex(graph.begin()->first);
            while (graph.size() < count)
                newVertex();
        }

        void resetVertices() {
            LOG_DEBUG("reset all vertices");
            for (int i = 0; i < getVertexCount(); i++)
                deleteVertex(graph.begin()->first);
            for (int i = 0; i < getVertexCount(); i++)
                newVertex();
        }

        void clearAllEdges() {
            for (auto &uIt : graph)
                uIt.second.clear();
            //std::cerr << "update called from clearAllEdges\n";
            updateConnectedComponent();
        }

        bool isAdjacent(Vertex* u, Vertex* v) {
            if (graph.find(u)->second.find(v) != graph.find(u)->second.end())
                return true;
            return false;
        }

        Edge* newEdge(Vertex* u, Vertex* v) {
            LOG_DEBUG("new edge between " + u->UUID + " and " + v->UUID);
            auto e = new Edge();
            if (graph.find(u)->second.find(v) == graph.find(u)->second.end())
                graph.find(u)->second.insert({v, std::unordered_set<Edge*>()});
            graph.find(u)->second.find(v)->second.insert(new Edge());
            //std::cerr << "update called from newEdge\n";
            updateConnectedComponent();
            //LOG_DEBUG("new edge " + u->UUID + " > " + v->UUID);
            u->component->pendingBlockCutTreeRebuild = true;
            return e;
        }

    };

};
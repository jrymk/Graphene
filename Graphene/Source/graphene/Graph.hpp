#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Structure.hpp"
#include "ConnectedComponent.hpp"
#include "../utils/Log.hpp"

namespace Graphene {

    class Graph {
    public:
        //                      u                           v        edge properties
        std::unordered_map<Vertex*, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>> graph;
        // keep v removed if no edges

        std::unordered_set<ConnectedComponent*> components;

        std::mutex mutex;

        Graph() = default;

        Vertex* debugVertexHighlight = nullptr;

        ConnectedComponent* newConnectedComponent(Vertex* root) {
            auto c = new ConnectedComponent(root);
            c->adjList.insert({root, std::unordered_set<Vertex*>()});
            components.insert(c);
            root->component = c;
            return c;
        }

        void updateConnectedComponent() {
            mutex.lock();

            for (auto &it : components) {
                if (it == nullptr)
                    std::cerr << "empty component\n";
            }

            std::unordered_map<Vertex*, std::unordered_set<Vertex*>> adjList;
            std::unordered_map<Vertex*, bool> visited;
            for (auto &vIt : graph) {
                visited.insert({vIt.first, false});
                adjList.insert({vIt.first, std::unordered_set<Vertex*>()});
            }
            for (auto &uIt : graph) {
                for (auto &vIt : uIt.second) {
                    adjList.find(uIt.first)->second.insert(vIt.first);
                    adjList.find(vIt.first)->second.insert(uIt.first);
                }
            }
            for (auto &it : components)
                it->updateConnectedComponent(adjList, visited);

            for (auto &it : components) {
                if (!it->isValidComponent())
                    deleteConnectedComponent(it, true);
            }

            for (auto &it : visited) {
                if (!it.second) {
                    auto c = newConnectedComponent(it.first);
                    c->updateConnectedComponent(adjList, visited);
                }
            }

            mutex.unlock();
        }

        void deleteConnectedComponent(ConnectedComponent* component, bool deleteContent) {
            if (!deleteContent) { // erase all edges
                for (auto &uIt : component->adjList) {
                    for (auto &vIt : component->adjList) {
                        graph.find(uIt.first)->second.erase(vIt.first);
                        uIt.second.erase(vIt.first);
                    }
                }
            }
            for (auto &it : component->adjList) {
                if (deleteContent) {
                    graph.erase(it.first);
                    for (auto &uIt : graph) {
                        for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++) {
                            if (vIt->first == it.first)
                                uIt.second.erase(vIt);
                        }
                    }
                } else
                    newConnectedComponent(it.first);
            }
            components.erase(component);
            delete component;
        }

        unsigned int getVertexCount() const {
            return graph.size();
        }

        Vertex* newVertex() {
            auto v = new Vertex(0);
            graph.insert({v, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>()});
            newConnectedComponent(v);
            return v;
        }

        void deleteEdge(Vertex* u, Vertex* v) {
            graph.find(u)->second.erase(v);
            graph.find(v)->second.erase(u);
            updateConnectedComponent();
        }

        void deleteVertex(Vertex* v) {
            if (v->component->getRootVertex() == v) {
                if (v->component->adjList.size() == 1) { // v is the only vertex in the component: delete the component (it should not have adjacent edges too)
                    components.erase(v->component);
                    delete v->component;
                } else { // transfer component ownership to random remaining vertex
                    for (auto &it : v->component->adjList) {
                        deleteEdge(it.first, v);
                        deleteEdge(v, it.first);
                    }
                    v->component->adjList.erase(v);
                    if (v == v->component->getRootVertex())
                        v->component->setRootVertex(v->component->adjList.begin()->first);
                }
            }
            graph.erase(v);
            for (auto &uIt : graph) {
                for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++) {
                    if (vIt->first == v)
                        uIt.second.erase(vIt);
                }
            }
            //delete v;
            updateConnectedComponent();
            //LOG_DEBUG("finished delete vertex");
        }

        void resizeVertices(int count) {
            updateConnectedComponent();
            while (graph.size() > count)
                deleteVertex(graph.begin()->first);
            while (graph.size() < count)
                newVertex();
        }

        void resetVertices() {
            for (int i = 0; i < getVertexCount(); i++)
                deleteVertex(graph.begin()->first);
            for (int i = 0; i < getVertexCount(); i++)
                newVertex();
        }

        void clearAllEdges() {
            for (auto &uIt : graph)
                uIt.second.clear();
            updateConnectedComponent();
        }

        bool isAdjacent(Vertex* u, Vertex* v) {
            if (graph.find(u)->second.find(v) != graph.find(u)->second.end())
                return true;
            return false;
        }

        Edge* newEdge(Vertex* u, Vertex* v) {
            auto e = new Edge();
            if (graph.find(u)->second.find(v) == graph.find(u)->second.end())
                graph.find(u)->second.insert({v, std::unordered_set<Edge*>()});
            graph.find(u)->second.find(v)->second.insert(new Edge());
            updateConnectedComponent();
            //LOG_DEBUG("new edge " + u->UUID + " > " + v->UUID);
            return e;
        }

    };

};
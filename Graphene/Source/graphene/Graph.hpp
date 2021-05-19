#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Structure.hpp"

namespace Graphene {

    class Graph {
    public:
        //                      u                           v        edge properties
        std::unordered_map<Vertex*, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>> graph;
        // keep v removed if no edges

        std::unordered_set<ConnectedComponent*> components;

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
            std::cerr << "update component started\n";
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
            /*
            // check for invalid components
            for (auto &it : components) {
                std::cerr << "QC pass 4/5\n";
                if (!it->validComponent) {
                    deleteConnectedComponent(it, false);
                }
            }
            */
            // check for dangling vertices
            for (auto &it : visited) {
                if (!it.second) {
                    auto c = newConnectedComponent(it.first);
                    c->updateConnectedComponent(adjList, visited);
                }
            }

            std::cerr << "update component ended\n";
        }

        void deleteConnectedComponent(ConnectedComponent* component, bool deleteContent) {
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
            //updateConnectedComponent();
            delete component;
        }

        unsigned int getVertexCount() const {
            return graph.size();
        }

        Vertex* newVertex() {
            std::cerr << "new vertex called\n";
            auto v = new Vertex(0);
            graph.insert({v, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>()});
            newConnectedComponent(v);
            updateConnectedComponent();
            return v;
        }

        void deleteVertex(Vertex* v) {
            std::cerr << "Delete vertex: " << v->component->UUID << "\n";
            if (v->component->root == v) {
                if (v->component->adjList.size() == 1) { // v is the only vertex in the component: delete the component
                    std::cerr << "delete mode 1\n";
                    components.erase(v->component);
                    delete v->component;
                } else { // transfer component ownership to random remaining vertex
                    std::cerr << "delete mode 2\n";
                    v->component->adjList.erase(v);
                    v->component->root = v->component->adjList.begin()->first;
                }
            }
            graph.erase(v);
            for (auto &uIt : graph) {
                for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++) {
                    if (vIt->first == v)
                        uIt.second.erase(vIt);
                }
            }
            updateConnectedComponent();
        }

        void resizeVertices(int count) {
            updateConnectedComponent();
            while (graph.size() > count)
                deleteVertex(graph.begin()->first);
            while (graph.size() < count)
                newVertex();
        }

        void resetVertices() {
            for (int i = 0; i < getVertexCount(); i++) {
                deleteVertex(graph.begin()->first);
                newVertex();
            }
        }

        void clearAllEdges() {
            for (auto &uIt : graph)
                uIt.second.clear();
            updateConnectedComponent();
        }

        void deleteEdge(Vertex* u, Vertex* v) {
            graph.find(u)->second.erase(v);
            graph.find(v)->second.erase(u);
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
            return e;
        }

    };

};
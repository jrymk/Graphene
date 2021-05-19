#pragma once

#include "Graph.hpp"

namespace Graphene {

    /* Utilities to iterate through graph
     *
     * Usage:
     * VertexIter it(&graph);
     * while (it.next()) {
     *     it.v ...
     * }
     */

    class VertexIter {
    private:
        Graph* graph;
        std::unordered_map<Vertex*, std::unordered_multimap<Vertex*, std::unordered_set<Edge*>>>::iterator vIt;

    public:
        explicit VertexIter(Graph* g) {
            graph = g;
            vIt = g->graph.begin();
        }

        Vertex* v = nullptr;

        bool next() {
            if (vIt == graph->graph.end())
                return false;

            v = vIt->first;
            vIt++;
            return true;
        }
    };

    class ComponentVertexIter {
    private:
        ConnectedComponent* component;
        std::unordered_map<Vertex*, std::unordered_set<Vertex*>>::iterator vIt;

    public:
        ComponentVertexIter(ConnectedComponent* c) {
            component = c;
            vIt = c->adjList.begin();
        }

        Vertex* v = nullptr;

        bool next() {
            if (vIt == component->adjList.end())
                return false;

            v = vIt->first;
            vIt++;
            return true;
        }
    };

    class AdjacentIter {
    private:
        Graph* graph;
        std::queue<std::pair<Vertex*, Vertex*>> edges;
    public:
        explicit AdjacentIter(Graph* g) {
            graph = g;

            for (auto &uIt : graph->graph) {
                for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++)
                    edges.push({uIt.first, vIt->first});
            }
        }

        Vertex* u = nullptr;
        Vertex* v = nullptr;

        bool next() {
            if (edges.empty()) return false;
            else {
                u = edges.front().first;
                v = edges.front().second;
                edges.pop();
                return true;
            }
        }
    };

    class EdgeIter {
    private:
        Graph* graph;
        std::queue<std::pair<std::pair<Vertex*, Vertex*>, Edge*>> edges;
    public:
        explicit EdgeIter(Graph* g) {
            graph = g;

            for (auto &uIt : graph->graph) {
                for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++) {
                    for (auto eIt = vIt->second.begin(); eIt != vIt->second.end(); eIt++)
                        edges.push({{uIt.first, vIt->first}, *eIt});
                }
            }
        }

        Vertex* u = nullptr;
        Vertex* v = nullptr;
        Edge* edge = nullptr;

        bool next() {
            if (edges.empty()) return false;
            else {
                u = edges.front().first.first;
                v = edges.front().first.second;
                edge = edges.front().second;
                edges.pop();
                return true;
            }
        }
    };

    class ComponentEdgeIter {
    private:
        ConnectedComponent* component;
        std::queue<std::pair<Vertex*, Vertex*>> edges;

    public:
        explicit ComponentEdgeIter(ConnectedComponent* c) {
            component = c;

            for (auto &uIt : component->adjList) {
                for (auto vIt = uIt.second.begin(); vIt != uIt.second.end(); vIt++)
                        edges.push({uIt.first, *vIt});
            }
        }

        Vertex* u = nullptr;
        Vertex* v = nullptr;

        bool next() {
            if (edges.empty()) return false;
            else {
                u = edges.front().first;
                v = edges.front().first;
                edges.pop();
                return true;
            }
        }
    };

}
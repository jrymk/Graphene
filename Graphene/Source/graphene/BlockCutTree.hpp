#pragma once

#include <vector>
#include <unordered_set>
#include <stack>
#include "Structure.hpp"

namespace Graphene {

    enum BiconnectedComponentType {
        BLOCK,
        ARTICULATION
    };

    /**
     * A biconnected component consists of several vertices including articulation,
     * but articulation will be in an independent object here, and the remaining parts
     * will be in the same object with the type BLOCK.
     *
     * By definition, adjacent articulations share a biconnected component without
     * non-articulation vertices, and the block will have no vertices, so note
     * that `vertices` may be empty.
     */

    class BiconnectedComponent {
    public:
        BiconnectedComponentType type;
        std::vector<Vertex*> vertices;

        explicit BiconnectedComponent(BiconnectedComponentType type) :
                type(type) {
        }

        void addVertex(Vertex* vertex) {
            vertices.emplace_back(vertex);
        }
    };

    class BlockCutTree {
    public:
        std::unordered_map<Vertex*, BiconnectedComponent*> bcc;
        std::unordered_map<BiconnectedComponent*, std::unordered_set<BiconnectedComponent*>> adjList;

        ~BlockCutTree() {
            for (auto &i : adjList) {
                delete i.first;
            }
        }

        void addEdge(BiconnectedComponent* u, BiconnectedComponent* v) {
            assert(u->type != v->type);
            adjList[u].insert(v);
            adjList[v].insert(u);
        }
    };

}

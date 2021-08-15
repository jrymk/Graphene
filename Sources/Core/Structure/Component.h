#ifndef COMPONENT_H
#define COMPONENT_H

#include <unordered_map>
#include <unordered_set>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Structure/BlockCutTree/BlockCutTree.h>

namespace gfn {

    class BlockCutTree;

    class Component {
    private:
        std::unordered_map<Vertex*, std::pair<std::unordered_set<Vertex*>,        // connectivity-based
                std::unordered_map<Vertex*, std::unordered_set<Edge*>>>> adjList; // directional-edge-based

        gfn::BlockCutTree* blockCutTree = nullptr;

    public:
        gfn::Uuid uuid;
        std::unordered_set<Vertex*> vertices;
        std::unordered_set<Edge*> edges;

        Vertex* root;

        Component();

        std::unordered_map<Vertex*, std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>>&
        getAdjList();

        /// @brief get all vertices that are adjacent to the given vertex the component (non directional)
        /// @returns a set of all the adjacent vertices
        std::unordered_set<Vertex*> getAdjacentVertices(Vertex* v);

        /// @brief check if vertices are adjacent to each other in the component (non directional)
        bool isAdjacent(Vertex* u, Vertex* v);

        gfn::BlockCutTree* getBlockCutTree();
        void rebuildBlockCutTree();
    };
}

#endif
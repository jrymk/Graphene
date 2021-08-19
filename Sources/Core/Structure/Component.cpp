#include "Component.h"
#include <Core/Structure/BlockCutTree/BlockCutTree.h>

namespace gfn {
    Component::Component() :
            uuid(gfn::createNil()),
            root(nullptr) {}

    std::unordered_map<Vertex*, std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>>&
    Component::getAdjList() {
        return adjList;
    }

    std::unordered_set<Vertex*> Component::getAdjacentVertices(Vertex* v) {
        auto vIt = adjList.find(v);
        if (vIt == adjList.end())
            return {};
        return vIt->second.first;
    }

    bool Component::isAdjacent(Vertex* u, Vertex* v) {
        auto uIt = adjList.find(u);
        if (uIt == adjList.end())
            return false;
        return uIt->second.first.find(v) != uIt->second.first.end();
    }

    gfn::BlockCutTree* Component::getBlockCutTree() { return blockCutTree; }

    void Component::rebuildBlockCutTree() {
        if (blockCutTree != nullptr) delete blockCutTree;
        blockCutTree = BlockCutTreeBuilder().build(this);
    }
}
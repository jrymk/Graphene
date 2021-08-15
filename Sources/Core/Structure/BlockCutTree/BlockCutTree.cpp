#include "BlockCutTree.h"

namespace gfn {

    BlockCutTree::~BlockCutTree() {
        for (auto i : bcc) delete i;
    }

    void BlockCutTree::addEdge(BiconnectedComponent *u, BiconnectedComponent *v) {
        adjList[u].emplace_back(v);
        adjList[v].emplace_back(u);
        bcc.insert(u);
        bcc.insert(v);
    }

    void BlockCutTree::setBCC(Vertex *v, BiconnectedComponent *b) {
        if(getBCC(v) != nullptr){
            getBCC(v)->vertices.erase(v);
        }
        vertexToBCC[v] = b;
        bcc.insert(b);
        b->vertices.insert(v);
    }

    std::unordered_set<Vertex *> BiconnectedComponent::getVertices() {
        return vertices;
    }

    int BiconnectedComponent::size() {
        return vertices.size();
    }

    BiconnectedComponent *BlockCutTree::getBCC(Vertex *v) {
        if (vertexToBCC.find(v) == vertexToBCC.end()) return nullptr;
        return vertexToBCC[v];
    }

    std::unordered_set<BiconnectedComponent *> BlockCutTree::getBCCs() {
        return bcc;
    }

    std::vector<BiconnectedComponent *> BlockCutTree::getAdjacentBCC(BiconnectedComponent *b) {
        return adjList[b];
    }

}
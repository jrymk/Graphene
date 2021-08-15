#include "BlockCutTree.h"

namespace gfn {

    void BlockCutTreeBuilder::addVertex(BiconnectedComponent *b, Vertex *v) {
        if (tree->getBCC(v) == nullptr) {
            tree->setBCC(v, b);
            return;
        }
        if (articulation.find(v) == articulation.end()) {
            BiconnectedComponent *old = tree->getBCC(v);
            articulation.insert(v);
            tree->setBCC(v, new BiconnectedComponent());
            tree->addEdge(old, tree->getBCC(v));
        }
        tree->addEdge(b, tree->getBCC(v));
    }

    void BlockCutTreeBuilder::dfs(Component *component, Vertex *now, Vertex *parent) {
        in[now] = low[now] = timeStamp++;
        vertexStack.push(now);
        int child = 0;
        for (Vertex *i : component->getAdjacentVertices(now)) {
            if (i == parent) continue;
            if (in.find(i) != in.end()) {
                low[now] = std::min(low[now], in[i]);
            } else {
                child++;
                dfs(component, i, now);
                low[now] = std::min(low[now], low[i]);

                if (low[i] >= in[now]) {
                    auto b = new BiconnectedComponent();
                    while (true) {
                        Vertex *v = vertexStack.top();
                        vertexStack.pop();
                        addVertex(b, v);
                        if (v == i) break;
                    }
                    addVertex(b, now);
                }
            }
        }
        // single vertex
        if (child == 0 && now == parent) addVertex(new BiconnectedComponent(), now);
    }

    BlockCutTree *BlockCutTreeBuilder::build(Component *component) {
        tree = new BlockCutTree();
        auto root = *component->vertices.begin();
        dfs(component, root, root);
        return tree;
    }

}
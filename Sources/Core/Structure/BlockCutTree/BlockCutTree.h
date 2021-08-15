#ifndef GRAPHENE_BLOCKCUTTREE_H
#define GRAPHENE_BLOCKCUTTREE_H

#include <map>
#include <vector>
#include <Core/Structure/Structure.h>
#include <stack>

namespace gfn {

    class BiconnectedComponent {
        friend class BlockCutTree;
    private:
        std::unordered_set<Vertex *> vertices;
    public:
        std::unordered_set<Vertex *> getVertices();

        int size();
    };

    class BlockCutTree {
        friend class BlockCutTreeBuilder;

    private:
        std::unordered_map<Vertex *, BiconnectedComponent *> vertexToBCC;
        std::unordered_set<BiconnectedComponent *> bcc;
        std::unordered_map<BiconnectedComponent *, std::vector<BiconnectedComponent *>> adjList;

        void addEdge(BiconnectedComponent *u, BiconnectedComponent *v);

        void setBCC(Vertex *v, BiconnectedComponent *b);

    public:
        ~BlockCutTree();

        BiconnectedComponent *getBCC(Vertex *v);

        std::unordered_set<BiconnectedComponent *> getBCCs();

        std::vector<BiconnectedComponent *> getAdjacentBCC(BiconnectedComponent *b);
    };

    class BlockCutTreeBuilder {
    private:
        std::vector<Vertex *> visited;
        std::unordered_map<Vertex *, int> in, low;
        int timeStamp = 0;
        std::stack<Vertex *> vertexStack;
        std::unordered_set<Vertex *> articulation;

        void addVertex(BiconnectedComponent *b, Vertex *v);

        void dfs(Component *component, Vertex *now, Vertex *parent);

        BlockCutTree *tree;

    public:
        BlockCutTree *build(Component *component);
    };

}

#endif //GRAPHENE_BLOCKCUTTREE_H

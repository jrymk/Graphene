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
     * but articulation will be in an independent object here, and the remain part
     * will be in the same object with the type BLOCK.
     *
     * In definition, adjacent articulations share a biconnected component without
     * non-articulation vertices, and the block will have no any vertex, so note
     * that `vertices` may be empty.
     */
    class BiconnectedComponent {
    public:
        BiconnectedComponentType type;
        std::vector<Vertex *> vertices;

        explicit BiconnectedComponent(BiconnectedComponentType type) :
                type(type) {
        }

        void addVertex(Vertex *vertex) {
            vertices.emplace_back(vertex);
        }
    };

    class BlockCutTree {
    public:
        std::unordered_map<Vertex *, BiconnectedComponent *> bcc;
        std::unordered_map<BiconnectedComponent *, std::unordered_set<BiconnectedComponent *>> adjList;

        ~BlockCutTree() {
            for (auto &i : adjList) {
                delete i.first;
            }
        }

        void addEdge(BiconnectedComponent *u, BiconnectedComponent *v) {
            assert(u->type != v->type);
            adjList[u].insert(v);
            adjList[v].insert(u);
        }
    };

    class BlockCutTreeBuilder {

        ConnectedComponent *component;

    public:
        /**
         * @param component the component to build a block cut tree
         */
        explicit BlockCutTreeBuilder(ConnectedComponent *component) : component(component) {

        }

        /**
         * Build a block cut tree with the connected component given
         * in the constructor. Should be called again after every update.
         * @return a new block cut tree
         */
        BlockCutTree *build() {
            delete blockCutTree;
            blockCutTree = new BlockCutTree();
            visited.clear();
            children.clear();
            in.clear();
            low.clear();
            articulation.clear();
            timeStamp = 0;
            while (!vertexStack.empty()) vertexStack.pop();
            if (component->adjList.empty()) return blockCutTree;
            auto root = component->adjList.begin()->first;
//            std::cerr << "root " << root->UUID << "\n";
            countChildren(root);
            dfs(root, root);
            return blockCutTree;
        }

    private:
        BlockCutTree *blockCutTree = nullptr;
        std::unordered_set<Vertex *> visited;
        std::unordered_map<Vertex *, int> children;
        std::unordered_map<Vertex *, int> in, low;
        std::unordered_set<Vertex *> articulation;
        int timeStamp = 0;

        std::stack<Vertex *> vertexStack;

        void countChildren(Vertex *now) {
//            std::cerr << "count " << now->UUID << "\n";
            children[now] = 0;
            visited.insert(now);
            for (Vertex *i : component->adjList[now]) {
                if (visited.find(i) != visited.end()) continue;
                countChildren(i);
                children[now]++;
            }
        }

        BiconnectedComponent *makeBlock(Vertex *until = nullptr) {
//            std::cerr << "makeblock\n";
            auto *bcc = new BiconnectedComponent(BiconnectedComponentType::BLOCK);
            blockCutTree->adjList[bcc];
            while (!vertexStack.empty()) {
                Vertex *x = vertexStack.top();
                if (articulation.find(x) == articulation.end()) {
                    blockCutTree->bcc[x] = bcc;
                    bcc->addVertex(x);
                } else blockCutTree->addEdge(bcc, blockCutTree->bcc[x]);
                vertexStack.pop();
                if (x == until) break;
            }
            return bcc;
        }

        BiconnectedComponent *makeArticulation(Vertex *now) {
            if (articulation.find(now) != articulation.end()) {
                return blockCutTree->bcc[now];
            }
//            std::cerr << "articulation " << now->UUID << std::endl;
            articulation.insert(now);
            auto bcc = new BiconnectedComponent(BiconnectedComponentType::ARTICULATION);
            blockCutTree->adjList[bcc];
            blockCutTree->bcc[now] = bcc;
            bcc->addVertex(now);
            return bcc;
        }

        void dfs(Vertex *now, Vertex *parent) {
            in[now] = low[now] = timeStamp++;
            vertexStack.push(now);
            for (Vertex *i : component->adjList[now]) {
                if (i == parent) continue;
                if (in.find(i) != in.end()) {
                    low[now] = std::min(low[now], in[i]);
                } else {
                    dfs(i, now);
                    low[now] = std::min(low[now], low[i]);

                    if ((now != parent && low[i] >= in[now]) ||
                        (now == parent && children[now] > 1)) {
                        auto bcc = makeBlock(i);
                        auto articulationBCC = makeArticulation(now);
                        blockCutTree->addEdge(bcc, articulationBCC);
                    }
                }
            }
            if (now == parent && children[now] == 1 && articulation.find(now) == articulation.end()) {
                makeBlock();
            }
        }

    };

}

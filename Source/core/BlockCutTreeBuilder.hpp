#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <stack>
#include "Structure.hpp"
#include "BlockCutTree.hpp"
#include "ConnectedComponent.hpp"
#include "ProfilerUtils.hpp"
#include "Log.hpp"

namespace gph {
    class BlockCutTreeBuilder {
        ConnectedComponent* component;

    public:
        /**
         * @param component the component to build a block cut tree
         */
        explicit BlockCutTreeBuilder(ConnectedComponent* component) : component(component) {

        }

        /**
         * Build a block cut tree with the connected component given
         * in the constructor. Should be called again after every update.
         */
        void build() {
            Utils::Timer timer;
            LOG_INFO("building block cut tree for component " + component->uid);

            delete component->blockCutTree;
            component->blockCutTree = new BlockCutTree();
            visited.clear();
            children.clear();
            in.clear();
            low.clear();
            articulation.clear();
            timeStamp = 0;
            while (!vertexStack.empty()) vertexStack.pop();
            if (component->adjList.empty()) {
                LOG_DEBUG("block cut tree built (took " + std::to_string(timer.getMicroseconds()) + "us)");
                return;
            }
            auto root = component->getRootVertex();
//            std::cerr << "root " << root->uid << "\n";
            countChildren(root);
            dfs(root, root);
            // stores the bcc to vertices mapping (actual)
            buildMapping();
            LOG_DEBUG("block cut tree built (took " + std::to_string(timer.getMicroseconds()) + "us)");
        }

    private:
        std::unordered_set<Vertex*> visited;
        std::unordered_map<Vertex*, int> children;
        std::unordered_map<Vertex*, int> in, low;
        std::unordered_set<Vertex*> articulation;
        int timeStamp = 0;

        std::stack<Vertex*> vertexStack;

        void countChildren(Vertex* now) {
//            std::cerr << "count " << now->uid << "\n";
            children[now] = 0;
            visited.insert(now);
            for (Vertex* i : component->adjList[now]) {
                if (visited.find(i) != visited.end()) continue;
                countChildren(i);
                children[now]++;
            }
        }

        BiconnectedComponent* makeBlock(Vertex* until = nullptr) {
//            std::cerr << "makeblock\n";
            auto* bcc = new BiconnectedComponent(BiconnectedComponentType::BLOCK);
            component->blockCutTree->adjList[bcc];
            while (!vertexStack.empty()) {
                Vertex* x = vertexStack.top();
                if (articulation.find(x) == articulation.end()) {
                    component->blockCutTree->bcc[x] = bcc;
                    bcc->addVertex(x);
                } else component->blockCutTree->addEdge(bcc, component->blockCutTree->bcc[x]);
                vertexStack.pop();
                if (x == until) break;
            }
            return bcc;
        }

        BiconnectedComponent* makeArticulation(Vertex* now) {
            if (articulation.find(now) != articulation.end()) {
                return component->blockCutTree->bcc[now];
            }
//            std::cerr << "articulation " << now->uid << std::endl;
            articulation.insert(now);
            auto bcc = new BiconnectedComponent(BiconnectedComponentType::ARTICULATION);
            component->blockCutTree->adjList[bcc];
            component->blockCutTree->bcc[now] = bcc;
            bcc->addVertex(now);
            return bcc;
        }

        void dfs(Vertex* now, Vertex* parent) {
            in[now] = low[now] = timeStamp++;
            vertexStack.push(now);
            for (Vertex* i : component->adjList[now]) {
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
                        component->blockCutTree->addEdge(bcc, articulationBCC);
                    }
                }
            }
            if (now == parent && children[now] == 1 && articulation.find(now) == articulation.end()) {
                makeBlock();
            }
        }

        void buildMapping() {
            component->blockCutTree->mapping.clear();
            for (auto &bcc : component->blockCutTree->adjList) {
                if (bcc.first->type == BiconnectedComponentType::BLOCK)
                    component->blockCutTree->mapping.insert({bcc.first, std::unordered_set<Vertex*>()});
            }
            for (auto &v : component->blockCutTree->bcc) {
                if (v.second->type == BiconnectedComponentType::BLOCK)
                    component->blockCutTree->mapping.find(v.second)->second.insert(v.first);
                else {
                    for (auto &c : component->blockCutTree->adjList.find(v.second)->second) {
                        if (c->type == BiconnectedComponentType::BLOCK)
                            component->blockCutTree->mapping.find(c)->second.insert(v.first);
                    }
                }
            }
        }

    };
}
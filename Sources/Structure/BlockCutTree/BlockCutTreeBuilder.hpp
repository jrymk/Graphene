#pragma once

#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <Structure/Vertex.hpp>
#include <Structure/Component/Component.hpp>
#include <Structure/BlockCutTree/BlockCutTree.hpp>
#include <Structure/BlockCutTree/BiconnectedComponent.hpp>
#include <Properties/Properties.hpp>
#include <Logging/Logging.hpp>

namespace gfn::structure {
// each component owns one block cut tree, rebuilt upon graph update (componentifier)
    class BlockCutTreeBuilder {
    private:
        Component* component;

    public:
        /// @param component the component to build a block cut tree with
        BlockCutTreeBuilder(Component* component) : component(component) {}

        /// @brief Build a block cut tree with the connected component given in the constructor. Should be called again
        /// after every update.
        bool build() {
            // delete old biconnected component objects and clear adj list
            component->getBlockCutTree().reset();

            visited.clear();
            children.clear();
            in.clear();
            low.clear();
            articulation.clear();
            timeStamp = 0;

            while (!vertexStack.empty())
                vertexStack.pop();
            if (component->getAdjList().empty()) {
                logMessage << "BlockCutTreeBuilder: Component {" << component->uuid << "} adjacency list is empty";
                logVerbose;
                return true;
            }
            auto root = component->root;
            logMessage << "BlockCutTreeBuilder: Component {" << component->uuid
            << "} building block cut tree with root vertex {" << root->props->uuid.value << "}";
            logVerbose;
            countChildren(root);
            dfs(root, root);
            // stores the bcc to vertices mapping (actual)
            buildMapping();
            return true;
        }

    private:
        std::unordered_set<Vertex*> visited;
        std::unordered_map<Vertex*, int> children;
        std::unordered_map<Vertex*, int> in, low;
        std::unordered_set<Vertex*> articulation;
        int timeStamp = 0;
        std::stack<Vertex*> vertexStack;

        void countChildren(Vertex* now) {
            children[now] = 0;
            visited.insert(now);
            for (Vertex* i : component->getAdjacentVertices(now)) {
                if (visited.find(i) != visited.end())
                    continue;
                countChildren(i);
                children[now]++;
            }
        }

        BiconnectedComponent* makeBlock(Vertex* until = nullptr) {
            auto* bcc = new BiconnectedComponent(BiconnectedComponent::Type::BLOCK);
            component->getBlockCutTree().adjList.insert({bcc, std::unordered_set<BiconnectedComponent*>()});
            while (!vertexStack.empty()) {
                Vertex* x = vertexStack.top();
                if (articulation.find(x) == articulation.end()) {
                    component->getBlockCutTree().vertexBcc.insert({x, bcc});
                    bcc->getVertices().insert(x);
                } else
                    component->getBlockCutTree().addEdge(bcc, component->getBlockCutTree().vertexBcc.find(x)->second);
                vertexStack.pop();
                if (x == until)
                    break;
            }
            return bcc;
        }

        BiconnectedComponent* makeArticulation(Vertex* now) {
            if (articulation.find(now) != articulation.end()) {
                return component->getBlockCutTree().vertexBcc[now];
            }
            articulation.insert(now);
            auto bcc = new BiconnectedComponent(BiconnectedComponent::Type::ARTICULATION);
            component->getBlockCutTree().adjList.insert({bcc, std::unordered_set<BiconnectedComponent*>()});
            component->getBlockCutTree().vertexBcc[now] = bcc;
            bcc->getVertices().insert(now);
            return bcc;
        }

        void dfs(Vertex* now, Vertex* parent) {
            in[now] = low[now] = timeStamp++;
            vertexStack.push(now);
            for (Vertex* i : component->getAdjacentVertices(now)) {
                if (i == parent)
                    continue;
                if (in.find(i) != in.end()) {
                    low[now] = std::min(low[now], in[i]);
                } else {
                    dfs(i, now);
                    low[now] = std::min(low[now], low[i]);

                    if ((now != parent && low[i] >= in[now]) || (now == parent && children[now] > 1)) {
                        auto bcc = makeBlock(i);
                        auto articulationBCC = makeArticulation(now);
                        component->getBlockCutTree().addEdge(bcc, articulationBCC);
                    }
                }
            }
            if (now == parent && children[now] == 1 && articulation.find(now) == articulation.end()) {
                makeBlock();
            }
        }

        void buildMapping() {
            for (auto& bcc : component->getBlockCutTree().adjList)
                bcc.first->getBlockVerticesIncludingArticulation().clear();
            for (auto& b : component->getBlockCutTree().vertexBcc) { // go through bccs of every vertex
                if (b.second->getType() == BiconnectedComponent::Type::BLOCK)
                    b.second->getBlockVerticesIncludingArticulation().insert(b.first);
                else {
                    for (auto& c : component->getBlockCutTree().adjList.find(b.second)->second) {
                        if (c->getType() == BiconnectedComponent::Type::BLOCK)
                            c->getBlockVerticesIncludingArticulation().insert(b.first);
                    }
                }
            }
        }
    };
} // namespace gfn::core::structure
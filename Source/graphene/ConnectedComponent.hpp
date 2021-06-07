#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include "imgui.h"
#include "../utils/UUIDGen.hpp"
#include "../utils/Log.hpp"
#include "BlockCutTree.hpp"

namespace Graphene {

    class ConnectedComponent {
    private:
        bool validComponent = false;
        Vertex* root;
        std::string UUID;

        Vec2f centroid = Vec2f(0.0f, 0.0f);
        Vec2f force = Vec2f(0.0f, 0.0f);
        double torque = 0.0;

    public:
        Vec2f center = Vec2f(0.0f, 0.0f);
        double radius = 0.0f;
        std::pair<Vec2f, Vec2f> bb = {{0.0f, 0.0f}, {0.0f, 0.0f}};
        std::pair<Vec2f, Vec2f> bbBack = {{0.0f, 0.0f}, {0.0f, 0.0f}};

        // a new baby needs a block cut tree too
        bool pendingBlockCutTreeRebuild = true;

        bool isValidComponent() const {
            return validComponent;
        }

        Vertex* getRootVertex() {
            return root;
        }

        void setRootVertex(Vertex* v) {
            root = v;
        }

        std::string getUUID() {
            return UUID;
        }

        void updateCentroid() {
            centroid = Vec2f(0.0f, 0.0f);
            for (auto &it : adjList) {
                auto vertex = it.first;
                centroid += vertex->getCoord();
            }
            centroid /= adjList.size();
        }

        void move(Vertex* v, Vec2f f) {
            Vec2f x = centroid;
            if(v != nullptr) v->getCoord() - centroid;
            force += f;
            torque += x * f;
        }

        void flushMove() {
            double theta = torque * Constants::c5;
            for (auto& it : adjList){
                auto vertex = it.first;
//                std::cerr << theta << " " << vertex->getCoord() << " ";
                Vec2f x = vertex->getCoord() - centroid;
                x = x.rotate(theta);
                vertex->setCoord(centroid + x);
//                std::cerr << vertex->getCoord() << "\n";
                vertex->directMove(force * Constants::c4);
            }
            torque = 0.0;
            force = Vec2f(0.0f, 0.0f);
        }

        ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        std::unordered_map<Vertex*, std::unordered_set<Vertex*>> adjList;

        BlockCutTree* blockCutTree = new BlockCutTree();

        explicit ConnectedComponent(Vertex* v) {
            root = v;
            ImGui::ColorConvertHSVtoRGB(genRandom() * 360.0f, 0.9f, 0.8f, color.x, color.y, color.z);
            UUID = Utils::UUIDGen::generate_64();
        }

        void updateConnectedComponent(
                std::unordered_map<Vertex*, std::unordered_set<Vertex*>>& graph,
                std::unordered_map<Vertex*, bool>& visited,
                Vertex* v) {
            v->component = this;
            visited.find(v)->second = true;
            adjList.insert({v, std::unordered_set<Vertex*>()});
            for (auto & it : (graph.find(v)->second)) {
                if (!visited.find(it)->second)
                    updateConnectedComponent(graph, visited, it);
            }
        }

        void updateConnectedComponent(
                std::unordered_map<Vertex*, std::unordered_set<Vertex*>>& graph,
                std::unordered_map<Vertex*, bool>& visited) {
            LOG_VERBOSE("updating component " + this->UUID);
            validComponent = !(visited.find(root)->second);

            if (!validComponent) {
                LOG_DEBUG("component root already covered by another component");
                adjList.clear();
                return;
            }
            adjList.clear();
            updateConnectedComponent(graph, visited, root);
            for (auto & uIt : adjList) {
                for (auto & vIt : (graph.find(uIt.first)->second)) {
                    adjList.find(uIt.first)->second.insert(vIt);
                    adjList.find(vIt)->second.insert(uIt.first);
                }
            }
        }

    };

}

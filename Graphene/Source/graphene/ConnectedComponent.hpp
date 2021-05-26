#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include "imgui.h"
#include "../utils/UUIDGen.hpp"
#include "BlockCutTree.hpp"

namespace Graphene {

    class ConnectedComponent {
    private:
        bool validComponent = false;
        Vertex* root;
        Vec2f position = Vec2f(0.0f, 0.0f);
        std::string UUID;

    public:
        Vec2f center = Vec2f(0.0f, 0.0f);
        double radius = 0.0f;
        std::pair<Vec2f, Vec2f> bb = {{0.0f, 0.0f}, {0.0f, 0.0f}};
        std::pair<Vec2f, Vec2f> bbBack = {{0.0f, 0.0f}, {0.0f, 0.0f}};

        bool isValidComponent() {
            return validComponent;
        }

        Vertex* getRootVertex() {
            return root;
        }

        void setRootVertex(Vertex* v) {
            root = v;
        }

        Vec2f getPosition() {
            return position;
        }

        void setPosition(Vec2f p) {
            position = p;
        }

        void changePosition(Vec2f p) {
            position += p;
        }

        std::string getUUID() {
            return UUID;
        }

        ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

        std::unordered_map<Vertex*, std::unordered_set<Vertex*>> adjList;
        BlockCutTree* blockCutTree;

        explicit ConnectedComponent(Vertex* v) {
            root = v;
            ImGui::ColorConvertHSVtoRGB(genRandom() * 360.0f, 0.9f, 0.8f, color.x, color.y, color.z);
            UUID = Utils::UUIDGen::generate_uuid_v4();
            blockCutTree = new BlockCutTree();
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
            validComponent = !(visited.find(root)->second);
            //std::cerr << "valid: " << validComponent << "\n";
            if (!validComponent) {
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
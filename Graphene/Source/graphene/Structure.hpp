#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include "imgui.h"
#include "../utils/UUIDGen.hpp"

namespace Graphene {

    class ConnectedComponent;

    struct Vec2f {
        float x;
        float y;

        Vec2f(float _x, float _y) {
            x = _x;
            y = _y;
        }

        /********* Operator Definitions *********
        *|	+ add
        *|	- subtract
        *|	* mul by number
        *|	/ divide by number
        *|	* dot
        *|	^ cross
        *****************************************/

        Vec2f operator+(Vec2f a) {
            return Vec2f(this->x + a.x, this->y + a.y);
        }

        void operator+=(Vec2f a) {
            this->x += a.x;
            this->y += a.y;
        }

        Vec2f operator-(Vec2f a) {
            return Vec2f(this->x - a.x, this->y - a.y);
        }

        void operator-=(Vec2f a) {
            this->x -= a.x;
            this->y -= a.y;
        }

        Vec2f operator*(float a) {
            return Vec2f(this->x * a, this->y * a);
        }

        void operator*=(float a) {
            this->x *= a;
            this->y *= a;
        }

        Vec2f operator/(float a) {
            return Vec2f(this->x / a, this->y / a);
        }

        void operator/=(float a) {
            this->x /= a;
            this->y /= a;
        }

        float operator*(Vec2f a) {
            return this->x * a.x + this->y * a.y;
        }

        float operator^(Vec2f a) {
            return this->x * a.y - this->y * a.x;
        }

        float length() {
            return sqrt(this->x * this->x + this->y * this->y);
        }

        Vec2f normalize() {
            if (length() <= 1e-9) return Vec2f(0, 0);
            return Vec2f(*this) / length();
        }

        friend std::ostream &operator<<(std::ostream &os, const Vec2f &nd) {
            return std::cout << "(" << nd.x << ", " << nd.y << ")";
        }
    };

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    bool launch = true;

    float genRandom() {
        if (launch) {
            launch = false;
            gen.seed(time(nullptr));
        }
        return dis(gen);
    }

    class Vertex {

    public:
        // TODO: fix up permissions
        bool pauseMove = false;
        int connectedComponent = 0;
        ConnectedComponent* component;
        std::string UUID;
        char name[1024] = "";

    private:
        int number = 0;
        bool nameVisible = false;
        std::string value = "";
        bool valueVisible = false;
        Vec2f coord = Vec2f(genRandom(), genRandom());
        Vec2f resultForce = Vec2f(0, 0);
        Vec2f normalized = Vec2f(genRandom(), genRandom());

    public:
        Vertex(int _num) {
            number = _num;
            UUID = Utils::UUIDGen::generate_uuid_v4();
        }

        Vec2f getCoord() {
            return coord;
        }

        void move(Vec2f v) {
            resultForce += v;
        }

        void flushMove(float c) {
            coord += resultForce * c;
            ////std::cerr << getNumber() << " " << resultForce * c << std::endl;

            resultForce = Vec2f(0, 0);
        }

        void directMove(Vec2f v) {
            coord += v;
        }

        int getNumber() {
            return number;
        }

        bool componentifier_visited = false;

    };

    class Edge {
    public:
        std::string name = "";
        bool nameVisible = false;
        std::string value = "";
        bool valueVisible = false;

        Edge() {

        }
    };

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

        explicit ConnectedComponent(Vertex* v) {
            root = v;
            ImGui::ColorConvertHSVtoRGB(genRandom() * 360.0f, 0.9f, 0.8f, color.x, color.y, color.z);
            UUID = Utils::UUIDGen::generate_uuid_v4();
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
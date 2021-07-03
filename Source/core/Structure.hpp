#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include "imgui.h"
#include "GenRandom.hpp"
#include "Constants.hpp"
#include "Vector.hpp"

namespace gph {

    class ConnectedComponent;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0.0, 1.0);
    bool launch = true;

    float genRandom() {
        if (launch) {
            launch = false;
            gen.seed(time(nullptr));
        }
        return distrib(gen);
    }

    class Vertex {

    public:
        // TODO: fix up permissions
        bool pauseMove = false;
        int connectedComponent = 0;
        ConnectedComponent* component;
        std::string uid;
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
            uid = Utils::UID::generate_64();
        }

        void setCoord(Vec2f newCoord) {
            coord = newCoord;
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

}
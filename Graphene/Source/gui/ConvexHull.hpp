#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "../graphene/Structure.hpp"

namespace Gui {
    class ConvexHull {

        std::vector<Graphene::Vec2f> points;

        std::vector<Graphene::Vec2f> output;

        ConvexHull() = default;

        void newPoint(Graphene::Vec2f p) {
            points.push_back(p);
        }

        void build() {

        }

    };
}
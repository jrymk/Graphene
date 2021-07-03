#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Common.hpp"

namespace gph {
    namespace GraphView {
        namespace Controls {

            void updateHoveredVertex() {
                Controls::hoveredVertex = nullptr;

                float closestVertexDistance = FLT_MAX;

                ::gph::VertexIter it(Graphene::core->getGraphObj());

                while (it.next()) {
                    float mouseVertexDistanceSquared =
                            powf(ImGui::GetIO().MousePos.x - View::mapToCanvas(it.v->getCoord()).x, 2.0f) +
                            powf(ImGui::GetIO().MousePos.y - View::mapToCanvas(it.v->getCoord()).y, 2.0f);

                    if (mouseVertexDistanceSquared <= pow(40.0 * pow(View::zoomTarget, 0.1), 2.0)) {
                        if (mouseVertexDistanceSquared <= closestVertexDistance) {
                            hoveredVertex = it.v;
                            closestVertexDistance = mouseVertexDistanceSquared;
                        }
                    }
                }

            }

            void updateHoveredComponent() {
                hoveredComponent = nullptr;

                for (auto c : Graphene::core->getComponents()) {
                    ::Gui::ConvexHull compConvexHull;
                    for (auto &v : c->adjList) {
                        for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 8.0f) {
                            compConvexHull.newPoint(
                                    {
                                            v.first->getCoord().x + View::mapToContext(float(20.0 * pow(View::zoomLevel, 0.1) + 30.0f) * cos(angle)),
                                            v.first->getCoord().y + View::mapToContext(float(20.0 * pow(View::zoomLevel, 0.1) + 30.0f) * sin(angle))
                                    }
                            );
                        }
                    }
                    compConvexHull.build();

                    std::vector<ImVec2> poly;
                    for (auto &p : compConvexHull.output)
                        poly.emplace_back(View::mapToCanvas(p));

                    if (Gui::isInsidePoly(poly, ImGui::GetIO().MousePos)) {
                        hoveredComponent = c;
                    }
                }

            }

            void updateMouse() {
                if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    leftMouseDownPos = ImGui::GetMousePos();
                    if (hoveredVertex != nullptr)
                        leftMouseDownVertex = hoveredVertex;
                }

                if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                    if (hoveredVertex == nullptr) {
                        if (leftMouseSelectionVertex != nullptr)
                            leftMouseSelectionVertex->pauseMove = false;
                        leftMouseSelectionVertex = nullptr;
                    }

                    if (leftMouseDownVertex != nullptr && hoveredVertex != nullptr) {
                        if (Graphene::core->getGraphObj()->isAdjacent(leftMouseDownVertex, hoveredVertex))
                            Graphene::core->getGraphObj()->deleteEdge(leftMouseDownVertex, hoveredVertex);
                        else if (Graphene::core->getGraphObj()->isAdjacent(hoveredVertex, leftMouseDownVertex))
                            Graphene::core->getGraphObj()->deleteEdge(hoveredVertex, leftMouseDownVertex);
                        else
                            Graphene::core->getGraphObj()->newEdge(leftMouseDownVertex, hoveredVertex);

                        Graphene::core->pendingInputUpdate = true;
                    }

                    if (((leftMouseDownPos.x - ImGui::GetMousePos().x) * (leftMouseDownPos.x - ImGui::GetMousePos().x)) +
                        ((leftMouseDownPos.y - ImGui::GetMousePos().y) * (leftMouseDownPos.y - ImGui::GetMousePos().y)) <= 25.0
                        && hoveredVertex == leftMouseDownVertex) {
                        if (leftMouseSelectionVertex == leftMouseDownVertex) {
                            if (leftMouseSelectionVertex != nullptr)
                                leftMouseSelectionVertex->pauseMove = false;
                            leftMouseSelectionVertex = nullptr;
                        } else {
                            if (leftMouseSelectionVertex != nullptr)
                                leftMouseSelectionVertex->pauseMove = false;
                            leftMouseSelectionVertex = leftMouseDownVertex;
                        }
                    }

                    leftMouseDownVertex = nullptr;
                }

                if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                    rightMouseDownPos = ImGui::GetMousePos();
                    if (hoveredVertex != nullptr)
                        rightMouseDownVertex = hoveredVertex;
                }

                if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                    if (hoveredVertex == nullptr)
                        rightMouseSelectionVertex = nullptr;

                    if (((rightMouseDownPos.x - ImGui::GetMousePos().x) * (rightMouseDownPos.x - ImGui::GetMousePos().x)) +
                        ((rightMouseDownPos.y - ImGui::GetMousePos().y) * (rightMouseDownPos.y - ImGui::GetMousePos().y)) <= 25.0) {
                        if (rightMouseSelectionVertex == rightMouseDownVertex)
                            rightMouseSelectionVertex = nullptr;
                        else
                            rightMouseSelectionVertex = rightMouseDownVertex;
                    }

                    rightMouseDownVertex = nullptr;
                }

                if (leftMouseSelectionVertex != nullptr)
                    leftMouseSelectionVertex->pauseMove = true;
            }

        }
    }
}
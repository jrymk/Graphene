#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../../utils/ProfilerUtils.hpp"

#include "View.hpp"
#include "Overlay.hpp"
#include "Renderer.hpp"

namespace Gui {
    namespace GraphView {

        Utils::Timer* animationTimer = new Utils::Timer();

        bool enableRender = true;
        bool enableAutoAdjustView = true;
        bool gridVisible = true;

        bool contextMenuOpen = false;
        bool forceOpenContextMenu = false;

        ::Graphene::Vertex* leftMouseDownVertex = nullptr;
        ::Graphene::Vertex* rightMouseDownVertex = nullptr;
        ::Graphene::Vertex* hoveredVertex = nullptr;
        ::Graphene::ConnectedComponent* hoveredComponent = nullptr;

        void autoAdjustView() {
            float x_max = -1000000000.0;
            float x_min = 1000000000.0;
            float y_max = -1000000000.0;
            float y_min = 1000000000.0;

            ::Graphene::VertexIter it(Graphene::graph);
            while (it.next()) {
                x_max = std::max(x_max, it.v->getCoord().x);
                x_min = std::min(x_min, it.v->getCoord().x);
                y_max = std::max(y_max, it.v->getCoord().y);
                y_min = std::min(y_min, it.v->getCoord().y);
            }

            //if (rightMouseDownVertex == nullptr) {
            if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                View::centerContext.x += ((x_max - x_min) / 2.0f + x_min - View::centerContext.x) * 0.2f;
                View::centerContext.y += ((y_max - y_min) / 2.0f + y_min - View::centerContext.y) * 0.2f;

                View::zoomTarget += (1.0f / std::max(std::max(x_max - x_min, y_max - y_min), 0.1f) - View::zoomLevel) * 0.2f;
            }
        }

        void updateKeyboardShortcuts() {
            // keyboard shortcut to toggle auto adjust view
            if (ImGui::IsKeyPressed('A', false))
                enableAutoAdjustView = !enableAutoAdjustView;
            if (ImGui::IsKeyPressed('V', true))
                Graphene::graph->newVertex();
        }

        void updateHoveredVertex() {
            hoveredVertex = nullptr;

            float closestVertexDistance = FLT_MAX;

            ::Graphene::VertexIter it(Graphene::graph);

            while (it.next()) {
                float mouseVertexDistanceSquared =
                        powf(ImGui::GetIO().MousePos.x - View::mapToCanvas(it.v->getCoord()).x, 2.0f) +
                        powf(ImGui::GetIO().MousePos.y - View::mapToCanvas(it.v->getCoord()).y, 2.0f);

                //if (ImGui::IsItemHovered()) {
                if (mouseVertexDistanceSquared <= powf(40.0f * powf(View::zoomTarget, 0.1), 2.0f)) {
                    if (mouseVertexDistanceSquared <= closestVertexDistance) {
                        hoveredVertex = it.v;
                        closestVertexDistance = mouseVertexDistanceSquared;
                    }
                }
                //}
            }


            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && hoveredVertex != nullptr) {
                leftMouseDownVertex = hoveredVertex;
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                if (leftMouseDownVertex != nullptr && hoveredVertex != nullptr) {
                    if (Graphene::graph->isAdjacent(leftMouseDownVertex, hoveredVertex))
                        Graphene::graph->deleteEdge(leftMouseDownVertex, hoveredVertex);
                    else if (Graphene::graph->isAdjacent(hoveredVertex, leftMouseDownVertex))
                        Graphene::graph->deleteEdge(hoveredVertex, leftMouseDownVertex);
                    else
                        Graphene::graph->newEdge(leftMouseDownVertex, hoveredVertex);

                    Graphene::core->pendingInputUpdate = true;
                }

                leftMouseDownVertex = nullptr;
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && hoveredVertex != nullptr) {
                rightMouseDownVertex = hoveredVertex;
                rightMouseDownVertex->pauseMove = true;
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && rightMouseDownVertex != nullptr) {
                rightMouseDownVertex->pauseMove = false;
                rightMouseDownVertex = nullptr;
            }
        }

        void updateHoveredComponent() {
            hoveredComponent = nullptr;

            for (auto c : Graphene::graph->components) {
                float mouseComponentDistanceSq =
                        powf(ImGui::GetIO().MousePos.x - View::mapToCanvas(c->center).x, 2.0f) +
                        powf(ImGui::GetIO().MousePos.y - View::mapToCanvas(c->center).y, 2.0f);

                //if (ImGui::IsItemHovered()) {
                if (mouseComponentDistanceSq <=
                    powf(c->radius * View::canvasFrameSize * View::zoomLevel + 50.0f * powf(View::zoomLevel, 0.1), 2.0f)) {
                    hoveredComponent = c;
                }
                //}
            }


            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && hoveredVertex != nullptr) {
                leftMouseDownVertex = hoveredVertex;
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                if (leftMouseDownVertex != nullptr && hoveredVertex != nullptr) {
                    if (Graphene::graph->isAdjacent(leftMouseDownVertex, hoveredVertex))
                        Graphene::graph->deleteEdge(leftMouseDownVertex, hoveredVertex);
                    else if (Graphene::graph->isAdjacent(hoveredVertex, leftMouseDownVertex))
                        Graphene::graph->deleteEdge(hoveredVertex, leftMouseDownVertex);
                    else
                        Graphene::graph->newEdge(leftMouseDownVertex, hoveredVertex);

                    Graphene::core->pendingInputUpdate = true;
                }

                leftMouseDownVertex = nullptr;
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && hoveredVertex != nullptr) {
                rightMouseDownVertex = hoveredVertex;
                rightMouseDownVertex->pauseMove = true;
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && rightMouseDownVertex != nullptr) {
                rightMouseDownVertex->pauseMove = false;
                rightMouseDownVertex = nullptr;
            }
        }

        void updateCamera() {
            if (View::isHovered)
                View::zoomTarget *= powf(1.05, ImGui::GetIO().MouseWheel);

            if (View::isHovered) {
                if (rightMouseDownVertex == nullptr) {
                    if (!(ImGui::GetMergedKeyModFlags() & ImGuiKeyModFlags_Ctrl) && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
                        View::centerContext.x += -ImGui::GetIO().MouseDelta.x / View::canvasFrameSize / View::zoomLevel;
                        View::centerContext.y -= -ImGui::GetIO().MouseDelta.y / View::canvasFrameSize / View::zoomLevel;
                    }
                }

                View::centerContext.x +=
                        -(ImGui::GetIO().MousePos.x - View::canvasOrigin.x
                          + (View::canvasOrigin.x - ImGui::GetIO().MousePos.x)
                            * (powf(1.05, ImGui::GetIO().MouseWheel))) / View::canvasFrameSize / View::zoomLevel;
                View::centerContext.y +=
                        (ImGui::GetIO().MousePos.y - View::canvasOrigin.y
                         + (View::canvasOrigin.y - ImGui::GetIO().MousePos.y)
                           * (powf(1.05, ImGui::GetIO().MouseWheel))) / View::canvasFrameSize / View::zoomLevel;
            }

            // smoothy transition zoom level
            View::zoomLevel *= powf(View::zoomTarget / View::zoomLevel, 0.1f);
            View::zoomLevel = std::max(View::zoomTarget, FLT_MIN);
        }

        void updateMouseCursor() {
            if (View::isHovered) {
                if (hoveredVertex != nullptr || leftMouseDownVertex != nullptr || rightMouseDownVertex != nullptr)
                    ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
                else
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
            }
        }

        void drawComponents() {
            for (auto &c : Graphene::graph->components) {
                ImVec4 col(0.0f, 0.0f, 0.0f, 0.28f);
                ImVec4 colHsv(0.0f, 0.0f, 0.0f, 0.0f);
                ImGui::ColorConvertRGBtoHSV(c->color.x, c->color.y, c->color.z, colHsv.x, colHsv.y, colHsv.z);
                colHsv.y = colHsv.y * 0.5f;
                ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);

                /*ImGui::GetWindowDrawList()->AddRectFilled(
                        {View::mapToCanvas(c->bb.first).x - 50.0f * powf(View::zoomLevel, 0.1),
                         View::mapToCanvas(c->bb.second).y - 50.0f * powf(View::zoomLevel, 0.1)},
                        {View::mapToCanvas(c->bb.second).x + 50.0f * powf(View::zoomLevel, 0.1),
                         View::mapToCanvas(c->bb.first).y + 50.0f * powf(View::zoomLevel, 0.1)},
                        ImGui::ColorConvertFloat4ToU32(col),
                        12.0f,
                        0
                );*/

                ImGui::GetWindowDrawList()->AddCircleFilled(
                        View::mapToCanvas(c->center),
                        c->radius * View::canvasFrameSize * View::zoomLevel + 50.0f * powf(View::zoomLevel, 0.1),
                        ImGui::ColorConvertFloat4ToU32(col),
                        0
                );

                /*colHsv.z = 1.0f - (1.0f - colHsv.z) * 0.5f;
                ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);
                col.w = 1.0f;
                ImGui::GetWindowDrawList()->AddText(
                        {View::mapToCanvas(c->bb.first).x - 50.0f * powf(View::zoomLevel, 0.1) + 4.0f,
                         View::mapToCanvas(c->bb.second).y - 50.0f * powf(View::zoomLevel, 0.1) - 18.0f},
                        ImGui::ColorConvertFloat4ToU32(col),
                        c->getUUID().c_str(),
                        nullptr
                );*/

            }

            if (hoveredComponent && !hoveredVertex && !leftMouseDownVertex && !rightMouseDownVertex) {
                ImGui::GetWindowDrawList()->AddCircle(
                        View::mapToCanvas(hoveredComponent->center),
                        hoveredComponent->radius * View::canvasFrameSize * View::zoomLevel + 50.0f * powf(View::zoomLevel, 0.1),
                        IM_COL32(150, 150, 255, 100),
                        0,
                        5.0f
                );
            }
        }

        void drawEdges() {
            // edge drawing
            ::Graphene::EdgeIter it(Graphene::graph);
            while (it.next()) {
                ImGui::GetWindowDrawList()->AddLine(
                        View::mapToCanvas(it.u->getCoord()),
                        View::mapToCanvas(it.v->getCoord()),
                        IM_COL32(200, 200, 200, 255), 5.0f * powf(View::zoomLevel, 0.1));
            }

            if (leftMouseDownVertex != nullptr) {
                if (hoveredVertex != nullptr) {
                    if (Graphene::graph->isAdjacent(leftMouseDownVertex, hoveredVertex) ||
                        Graphene::graph->isAdjacent(hoveredVertex, leftMouseDownVertex)) {
                        // disconnecting vertices
                        ImGui::GetWindowDrawList()->AddLine(
                                View::mapToCanvas(leftMouseDownVertex->getCoord()),
                                View::mapToCanvas(hoveredVertex->getCoord()),
                                IM_COL32(200, 0, 0, 120), 10.0f * powf(View::zoomLevel, 0.1));
                    } else {
                        // connecting vertices
                        ImGui::GetWindowDrawList()->AddLine(
                                View::mapToCanvas(leftMouseDownVertex->getCoord()),
                                View::mapToCanvas(hoveredVertex->getCoord()),
                                IM_COL32(0, 255, 0, 120), 5.0f * powf(View::zoomLevel, 0.1));
                    }
                } else {
                    ImGui::GetWindowDrawList()->AddLine(
                            View::mapToCanvas(leftMouseDownVertex->getCoord()),
                            ImGui::GetIO().MousePos,
                            IM_COL32(0, 255, 0, 60), 5.0f * powf(View::zoomLevel, 0.1));
                }
            }
        }

        void drawVertices() {
            for (auto &component : Graphene::graph->components) {
                ::Graphene::ComponentVertexIter it(component);
                while (it.next()) {
                    ImVec2 vertexScreenCoord(
                            View::canvasOrigin.x - (View::centerContext.x - it.v->getCoord().x) * View::canvasFrameSize * View::zoomLevel,
                            View::canvasOrigin.y + (View::centerContext.y - it.v->getCoord().y) * View::canvasFrameSize * View::zoomLevel);

                    //ImGui::GetWindowDrawList()->AddCircleFilled(vertexScreenCoord, 20.0f * powf(View::zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f),
                    //                                            (it.v == rightMouseDownVertex) ? IM_COL32(255, 221, 51, 255) : IM_COL32(255, 211, 0, 255));
                    ImGui::GetWindowDrawList()->AddCircleFilled(
                            vertexScreenCoord,
                            20.0f * powf(View::zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f),
                            (it.v == rightMouseDownVertex) ? ImGui::ColorConvertFloat4ToU32(component->color)
                                                           : ImGui::ColorConvertFloat4ToU32(component->color));

                    ImGui::PushFont(Gui::vertexTextFont);
                    ImGui::SetWindowFontScale(
                            (36.0f / 54.0f) * powf(View::zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255));

                    ImVec2 labelCenterPos(
                            View::canvasOrigin.x - (View::centerContext.x - it.v->getCoord().x) * View::canvasFrameSize * View::zoomLevel,
                            View::canvasOrigin.y + (View::centerContext.y - it.v->getCoord().y) * View::canvasFrameSize * View::zoomLevel);
                    ImVec2 labelMinPos(labelCenterPos.x - 100.0f, labelCenterPos.y - 100.0f);
                    ImVec2 labelMaxPos(labelCenterPos.x + 100.0f, labelCenterPos.y + 100.0f);
                    char* label = it.v->name;
                    ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
                    ImVec2 labelAlign(0.5f, 0.5f);
                    const ImRect bb(labelMinPos, labelMaxPos);
                    ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, 0, &labelSize, labelAlign, &bb);

                    ImGui::PopStyleColor(1);
                    ImGui::SetWindowFontScale(1.0f);
                    ImGui::PopFont();

                    /*if (it.v == component->getRootVertex()) {
                        ImGui::SetWindowFontScale(0.75f);
                        ImGui::GetWindowDrawList()->AddCircle(
                                vertexScreenCoord,
                                25.0f * powf(View::zoomLevel, 0.1),
                                IM_COL32(255, 255, 0, 255), 0, 3.0f * powf(View::zoomLevel, 0.1));

                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
                        ImVec2 labelCenterPos(
                                View::canvasOrigin.x - (View::centerContext.x - it.v->getCoord().x) * View::canvasFrameSize * View::zoomLevel,
                                View::canvasOrigin.y + (View::centerContext.y - it.v->getCoord().y) * View::canvasFrameSize * View::zoomLevel + 35.0f * powf(View::zoomLevel, 0.1));
                        ImVec2 labelMinPos(labelCenterPos.x - 100.0f, labelCenterPos.y - 50.0f);
                        ImVec2 labelMaxPos(labelCenterPos.x + 100.0f, labelCenterPos.y + 50.0f);
                        std::string tempStr(component->getUUID());
                        char* label = new char[tempStr.length() + 1];
                        strcpy(label, tempStr.c_str());
                        ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
                        ImVec2 labelAlign(0.5f, 0.5f);
                        const ImRect bb(labelMinPos, labelMaxPos);
                        ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, 0, &labelSize, labelAlign, &bb);
                        ImGui::PopStyleColor(1);
                        ImGui::SetWindowFontScale(1.0f);
                    }*/

                    if (it.v == Graphene::graph->debugVertexHighlight) {
                        ImGui::GetWindowDrawList()->AddCircleFilled(
                                vertexScreenCoord,
                                20.0f * powf(View::zoomLevel, 0.1) + 12.0f,
                                ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.4f))
                        );
                    }
                }
            }

            if (hoveredVertex != nullptr) {
                ImGui::GetWindowDrawList()->AddCircle(
                        ImVec2(View::canvasOrigin.x -
                               (View::centerContext.x - hoveredVertex->getCoord().x) * View::canvasFrameSize * View::zoomLevel,
                               View::canvasOrigin.y +
                               (View::centerContext.y - hoveredVertex->getCoord().y) * View::canvasFrameSize * View::zoomLevel
                        ),
                        25.0f * powf(View::zoomLevel, 0.1), IM_COL32(150, 150, 255, 100), 0, 5.0f * powf(View::zoomLevel, 0.1));
            }
        }

        void updateContextMenu() {
            contextMenuOpen = false;
            if (ImGui::GetMergedKeyModFlags() & ImGuiKeyModFlags_Ctrl | forceOpenContextMenu) {
                forceOpenContextMenu = false;
                if (ImGui::BeginPopupContextItem()) {
                    contextMenuOpen = true;
                    if (hoveredVertex != nullptr) {
                        ImGui::Text(("Vertex: " + hoveredVertex->UUID).c_str());
                        if (ImGui::Button("Delete")) {
                            Graphene::graph->deleteVertex(hoveredVertex);
                            contextMenuOpen = false;
                        }
                        ImGui::InputText("Vertex name", hoveredVertex->name, 1024, 0, 0, 0);
                        if (ImGui::IsItemFocused())
                            forceOpenContextMenu = true;
                    } else if (hoveredComponent != nullptr) {
                        ImGui::Text(("Component: " + hoveredComponent->getUUID()).c_str());
                        if (ImGui::Button("Disassemble")) {
                            Graphene::graph->deleteConnectedComponent(hoveredComponent, false);
                            contextMenuOpen = false;
                        }
                        if (ImGui::Button("Delete")) {
                            Graphene::graph->deleteConnectedComponent(hoveredComponent, true);
                            contextMenuOpen = false;
                        }
                        ImGui::ColorPicker3("Color", &hoveredComponent->color.x, ImGuiColorEditFlags_HSV);
                    } else
                        ImGui::Text("No selected object");

                    ImGui::EndPopup();
                }
            }
        }

        void show(::Graphene::Core* core, ::Graphene::Graph* graph) {
            Graphene::core = core;
            Graphene::graph = graph;

            ImGui::SetNextWindowSizeConstraints(ImVec2(300, 350), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin(u8"Graph View", 0, ImGuiWindowFlags_NoCollapse);

            ImGui::Checkbox("Enable render", &enableRender);
            ImGui::SameLine();
            ImGui::Checkbox("Show grid", &gridVisible);
            ImGui::SameLine();
            ImGui::Checkbox("Auto adjust view (A)", &enableAutoAdjustView);

            if (!enableAutoAdjustView) {
                ImGui::SameLine();
                if (ImGui::Button("Reset view")) {
                    View::zoomTarget = 1.0f;
                    View::centerContext = ImVec2(0.5f, 0.5f);
                }
            }

            View::canvasBegin();

            //Graphene::graph->updateConnectedComponent(); // continuously running this command is not a good idea

            if (enableAutoAdjustView)
                autoAdjustView();
            if (!contextMenuOpen)
                updateKeyboardShortcuts();

            if (!contextMenuOpen) {
                updateHoveredVertex();
                if (rightMouseDownVertex != nullptr) {
                    rightMouseDownVertex->directMove(
                            ::Graphene::Vec2f(
                                    ImGui::GetIO().MouseDelta.x / View::canvasFrameSize / View::zoomLevel,
                                    -ImGui::GetIO().MouseDelta.y / View::canvasFrameSize / View::zoomLevel
                            )
                    );
                    rightMouseDownVertex->flushMove(0.0f);
                }
                updateHoveredComponent();
            }

            if (enableRender) {
                updateCamera();
                updateMouseCursor();

                if (gridVisible)
                    Renderer::drawGrid();

                drawComponents();
                drawEdges();
                drawVertices();
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 100, 100, 255));
                ImGui::PushFont(Gui::framerateTextFont);
                std::string tempStr("Rendering is disabled");
                char* label = new char[tempStr.length() + 1];
                strcpy(label, tempStr.c_str());
                ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
                ImVec2 labelAlign(0.5f, 0.5f);
                const ImRect bb(View::canvasCursor, {View::canvasCursor.x + View::canvasSize.x, View::canvasCursor.y + View::canvasSize.y});

                ImGui::RenderTextClipped(bb.Min, bb.Max, label, 0, &labelSize, labelAlign, &bb);
                ImGui::PopStyleColor(1);
                ImGui::PopFont();
            }

            updateContextMenu();
            Overlay::draw();

            View::canvasEnd();
        }
    }
}
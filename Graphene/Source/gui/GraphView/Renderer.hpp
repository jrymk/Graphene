#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../../utils/ProfilerUtils.hpp"

#include "Common.hpp"
#include "RenderUtils.hpp"

namespace Gui {
    namespace GraphView {
        namespace Renderer {

            void drawGrid() {
                RenderUtils::drawGridLines(
                        true,
                        View::mapToCanvas(0.0, 0.0).x,
                        View::mapToCanvas(pow(10, -(int) ceil(log10(View::zoomLevel)))),
                        -View::canvasSize.x / 2.0f,
                        View::canvasSize.x / 2.0f,
                        -View::canvasSize.y / 2.0f,
                        View::canvasSize.y / 2.0f,
                        IM_COL32(90, 90, 90, 80),
                        2.0
                );
                RenderUtils::drawGridLines(
                        true,
                        View::mapToCanvas(0.0, 0.0).x,
                        View::mapToCanvas(pow(10, -(int) ceil(log10(View::zoomLevel)) - 1)),
                        -View::canvasSize.x / 2.0f,
                        View::canvasSize.x / 2.0f,
                        -View::canvasSize.y / 2.0f,
                        View::canvasSize.y / 2.0f,
                        IM_COL32(90, 90, 90, 80),
                        1.0
                );
                RenderUtils::drawGridLines(
                        false,
                        View::mapToCanvas(0.0, 0.0).y,
                        View::mapToCanvas(pow(10, -(int) ceil(log10(View::zoomLevel)))),
                        -View::canvasSize.y / 2.0f,
                        View::canvasSize.y / 2.0f,
                        -View::canvasSize.x / 2.0f,
                        View::canvasSize.x / 2.0f,
                        IM_COL32(90, 90, 90, 80),
                        2.0
                );
                RenderUtils::drawGridLines(
                        false,
                        View::mapToCanvas(0.0, 0.0).y,
                        View::mapToCanvas(pow(10, -(int) ceil(log10(View::zoomLevel)) - 1)),
                        -View::canvasSize.y / 2.0f,
                        View::canvasSize.y / 2.0f,
                        -View::canvasSize.x / 2.0f,
                        View::canvasSize.x / 2.0f,
                        IM_COL32(90, 90, 90, 80),
                        1.0
                );
            }

            void drawComponents() {
                for (auto &c : Graphene::core->getGraphObj()->components) {
                    /*ImVec4 col(0.0f, 0.0f, 0.0f, 0.28f);
                    ImVec4 colHsv(0.0f, 0.0f, 0.0f, 0.0f);
                    ImGui::ColorConvertRGBtoHSV(c->color.x, c->color.y, c->color.z, colHsv.x, colHsv.y, colHsv.z);
                    colHsv.y = colHsv.y * 0.5f;
                    ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);*/

                    // draw component bounding circle
                    /*ImGui::GetWindowDrawList()->AddCircleFilled(
                            View::mapToCanvas(c->center),
                            float(c->radius * View::canvasFrameSize * View::zoomLevel + 50.0 * pow(View::zoomLevel, 0.1)),
                            ImGui::ColorConvertFloat4ToU32(col),
                            0
                    );*/
                }

                if (Controls::hoveredComponent && !Controls::hoveredVertex && !Controls::leftMouseDownVertex && !Controls::rightMouseDownVertex) {
                    /*ImGui::GetWindowDrawList()->AddCircle(
                            View::mapToCanvas(Controls::hoveredComponent->center),
                            float(Controls::hoveredComponent->radius * View::canvasFrameSize * View::zoomLevel + 50.0 * pow(View::zoomLevel, 0.1)),
                            IM_COL32(150, 150, 255, 100),
                            0,
                            5.0f
                    );*/
                    ImVec4 col(0.0f, 0.0f, 0.0f, 0.28f);
                    ImVec4 colHsv(0.0f, 0.0f, 0.0f, 0.0f);
                    ImGui::ColorConvertRGBtoHSV(Controls::hoveredComponent->color.x, Controls::hoveredComponent->color.y, Controls::hoveredComponent->color.z,
                                                colHsv.x, colHsv.y, colHsv.z);
                    colHsv.y = colHsv.y * 0.5f;
                    ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);

                    ImGui::GetWindowDrawList()->AddCircleFilled(
                            View::mapToCanvas(Controls::hoveredComponent->center),
                            float(Controls::hoveredComponent->radius * View::canvasFrameSize * View::zoomLevel + 50.0 * pow(View::zoomLevel, 0.1)),
                            ImGui::ColorConvertFloat4ToU32(col),
                            0
                    );
                }
            }

            void drawEdges() {
                // edge drawing
                ::Graphene::EdgeIter it(Graphene::core->getGraphObj());
                while (it.next()) {
                    ImGui::GetWindowDrawList()->AddLine(
                            View::mapToCanvas(it.u->getCoord()),
                            View::mapToCanvas(it.v->getCoord()),
                            IM_COL32(200, 200, 200, 255), float(5.0 * pow(View::zoomLevel, 0.1)));
                }

                if (Controls::leftMouseDownVertex != nullptr) {
                    if (Controls::hoveredVertex != nullptr) {
                        if (Graphene::core->getGraphObj()->isAdjacent(Controls::leftMouseDownVertex, Controls::hoveredVertex) ||
                            Graphene::core->getGraphObj()->isAdjacent(Controls::hoveredVertex, Controls::leftMouseDownVertex)) {
                            // disconnecting vertices
                            ImGui::GetWindowDrawList()->AddLine(
                                    View::mapToCanvas(Controls::leftMouseDownVertex->getCoord()),
                                    View::mapToCanvas(Controls::hoveredVertex->getCoord()),
                                    IM_COL32(200, 0, 0, 120), float(10.0 * pow(View::zoomLevel, 0.1)));
                        } else {
                            // connecting vertices
                            ImGui::GetWindowDrawList()->AddLine(
                                    View::mapToCanvas(Controls::leftMouseDownVertex->getCoord()),
                                    View::mapToCanvas(Controls::hoveredVertex->getCoord()),
                                    IM_COL32(0, 255, 0, 120), float(5.0 * pow(View::zoomLevel, 0.1)));
                        }
                    } else {
                        ImGui::GetWindowDrawList()->AddLine(
                                View::mapToCanvas(Controls::leftMouseDownVertex->getCoord()),
                                ImGui::GetIO().MousePos,
                                IM_COL32(0, 255, 0, 60), float(5.0 * pow(View::zoomLevel, 0.1)));
                    }
                }
            }

            void drawVertices() {
                for (auto &component : Graphene::core->getGraphObj()->components) {
                    ::Graphene::ComponentVertexIter it(component);
                    while (it.next()) {
                        ImVec2 vertexScreenCoord(
                                float(View::canvasOrigin.x - (View::centerContext.x - it.v->getCoord().x) * View::canvasFrameSize * View::zoomLevel),
                                float(View::canvasOrigin.y + (View::centerContext.y - it.v->getCoord().y) * View::canvasFrameSize * View::zoomLevel));

                        //ImGui::GetWindowDrawList()->AddCircleFilled(vertexScreenCoord, 20.0f * powf(View::zoomLevel, 0.1) * ((it.v == Controls::rightMouseDownVertex) ? 1.1f : 1.0f),
                        //                                            (it.v == Controls::rightMouseDownVertex) ? IM_COL32(255, 221, 51, 255) : IM_COL32(255, 211, 0, 255));
                        ImGui::GetWindowDrawList()->AddCircleFilled(
                                vertexScreenCoord,
                                float(20.0 * pow(View::zoomLevel, 0.1) * ((it.v == Controls::rightMouseDownVertex) ? 1.1 : 1.0)),
                                (it.v == Controls::rightMouseDownVertex) ? ImGui::ColorConvertFloat4ToU32(component->color)
                                                                         : ImGui::ColorConvertFloat4ToU32(component->color));

                        ImGui::PushFont(Gui::vertexTextFont);
                        ImGui::SetWindowFontScale(
                                float((36.0 / 54.0) * pow(View::zoomLevel, 0.1) * ((it.v == Controls::rightMouseDownVertex) ? 1.1 : 1.0)));
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255));

                        ImVec2 labelCenterPos(
                                float(View::canvasOrigin.x - (View::centerContext.x - it.v->getCoord().x) * View::canvasFrameSize * View::zoomLevel),
                                float(View::canvasOrigin.y + (View::centerContext.y - it.v->getCoord().y) * View::canvasFrameSize * View::zoomLevel));
                        ImVec2 labelMinPos(labelCenterPos.x - 100.0f, labelCenterPos.y - 100.0f);
                        ImVec2 labelMaxPos(labelCenterPos.x + 100.0f, labelCenterPos.y + 100.0f);
                        char* label = it.v->name;
                        ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);
                        ImVec2 labelAlign(0.5f, 0.5f);
                        const ImRect bb(labelMinPos, labelMaxPos);
                        ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, nullptr, &labelSize, labelAlign, &bb);

                        ImGui::PopStyleColor(1);
                        ImGui::SetWindowFontScale(1.0f);
                        ImGui::PopFont();

                        if (it.v == Graphene::core->getGraphObj()->debugVertexHighlight) {
                            ImGui::GetWindowDrawList()->AddCircleFilled(
                                    vertexScreenCoord,
                                    float(20.0 * pow(View::zoomLevel, 0.1) + 12.0),
                                    ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.4f))
                            );
                        }

                        if (it.v == Controls::leftMouseSelectionVertex) {
                            ImGui::GetWindowDrawList()->AddCircle(
                                    vertexScreenCoord,
                                    float(20.0 * pow(View::zoomLevel, 0.1) + 12.0),
                                    ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)),
                                    0,
                                    2.0
                            );
                        }
                    }
                }

                if (Controls::hoveredVertex != nullptr) {
                    ImGui::GetWindowDrawList()->AddCircle(
                            ImVec2(float(View::canvasOrigin.x -
                                   (View::centerContext.x - Controls::hoveredVertex->getCoord().x) * View::canvasFrameSize * View::zoomLevel),
                                   float(View::canvasOrigin.y +
                                   (View::centerContext.y - Controls::hoveredVertex->getCoord().y) * View::canvasFrameSize * View::zoomLevel)
                            ),
                            float(25.0 * pow(View::zoomLevel, 0.1)),
                            IM_COL32(150, 150, 255, 100),
                            0,
                            float(5.0 * pow(View::zoomLevel, 0.1)));
                }
            }

        }
    }
}
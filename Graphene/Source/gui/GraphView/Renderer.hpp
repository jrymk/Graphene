#pragma once

#include <cmath>
#include "../Window.hpp"
#include "Common.hpp"
#include "../../graphene/BlockCutTree.hpp"
#include "RenderUtils/ConvexHull.hpp"
#include "RenderUtils/AlignedText.hpp"
#include "RenderUtils/DrawPolygon.hpp"

namespace Gui {
    namespace GraphView {
        namespace Renderer {

            void drawGridLines(bool vertical, double origin, double spacing,
                               float perpBegin, float perpEnd, float parBegin, float parEnd, ImColor col, float thickness) {
                const float canvas = vertical ? View::canvasOrigin.x : View::canvasOrigin.y;
                const double snapGrid = origin + round((canvas - origin) / spacing) * spacing;
                int lineDrawLimit = 500;
                for (int i = int(perpBegin / spacing);
                     i <= int(perpEnd / spacing); i++) {
                    if (vertical) {
                        ImGui::GetWindowDrawList()->AddLine(
                                ImVec2(float(snapGrid + i * spacing), View::canvasOrigin.y + parBegin),
                                ImVec2(float(snapGrid + i * spacing), View::canvasOrigin.y + parEnd),
                                col,
                                thickness
                        );
                    } else {
                        ImGui::GetWindowDrawList()->AddLine(
                                ImVec2(View::canvasOrigin.x + parBegin, float(snapGrid + i * spacing)),
                                ImVec2(View::canvasOrigin.x + parEnd, float(snapGrid + i * spacing)),
                                col,
                                thickness
                        );
                    }
                    if (!(--lineDrawLimit))
                        break;
                }
            }

            void drawGrid() {
                drawGridLines(
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
                drawGridLines(
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
                drawGridLines(
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
                drawGridLines(
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

                    Gui::drawPolygon(poly, IM_COL32(100, 100, 100, 100));

                    int bccId = 0;
                    for (auto &bcc : c->blockCutTree->mapping) {
                        ::Gui::ConvexHull convexHull;

                        ImVec4 col(0.0f, 0.0f, 0.0f, 0.30f);
                        ImVec4 colHsv(float(float(bccId) / c->blockCutTree->mapping.size()), 0.5f,  1.0f, 0.0f);
                        ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);

                        for (auto &v : bcc.second) {
                            for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 8.0f) {
                                convexHull.newPoint(
                                        {
                                                v->getCoord().x + View::mapToContext(float(20.0 * pow(View::zoomLevel, 0.1) + 15.0f) * cos(angle)),
                                                v->getCoord().y + View::mapToContext(float(20.0 * pow(View::zoomLevel, 0.1) + 15.0f) * sin(angle))
                                        }
                                );
                            }
                        }
                        convexHull.build();

                        std::vector<ImVec2> poly;
                        for (auto &p : convexHull.output)
                            poly.emplace_back(View::mapToCanvas(p));

                        Gui::drawPolygon(poly, ImGui::ColorConvertFloat4ToU32(col));

                        bccId++;
                    }

                }

                if (Controls::hoveredComponent && !Controls::hoveredVertex && !Controls::leftMouseDownVertex && !Controls::rightMouseDownVertex) {
                    /*ImGui::GetWindowDrawList()->AddCircle(
                            View::mapToCanvas(Controls::hoveredComponent->center),
                            float(Controls::hoveredComponent->radius * View::canvasFrameSize * View::zoomLevel + 50.0 * pow(View::zoomLevel, 0.1)),
                            IM_COL32(150, 150, 255, 100),
                            0,
                            5.0f
                    );*/
                    /*ImVec4 col(0.0f, 0.0f, 0.0f, 0.28f);
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
                    );*/
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
                                ImGui::ColorConvertFloat4ToU32(component->color)
                        );

                        ::Gui::AlignedText(
                                View::mapToCanvas(it.v->getCoord()),
                                {0.5f, 0.5f},
                                {200.0f, 100.0f},
                                it.v->name,
                                Gui::vertexTextFont,
                                float((36.0 / 54.0) * pow(View::zoomLevel, 0.1) * ((it.v == Controls::rightMouseDownVertex) ? 1.1 : 1.0)),
                                IM_COL32(15, 15, 15, 255)
                        );

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
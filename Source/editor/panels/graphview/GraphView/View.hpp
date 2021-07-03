#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "../../../../core/Structure.hpp"
#include "Common.hpp"

namespace gph {
    namespace GraphView {
        namespace View {

            double mapToContext(double l) {
                return l / canvasFrameSize / zoomLevel;
            }

            double mapToCanvas(double l) {
                return l * canvasFrameSize * zoomLevel;
            }

            /*::Graphene::Vec2f mapToContext(ImVec2 c) {
                return {canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel,
                        canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel};
            }*/

            ImVec2 mapToCanvas(double x, double y) {
                return {float(canvasOrigin.x - (centerContext.x - x) * canvasFrameSize * zoomLevel),
                        float(canvasOrigin.y + (centerContext.y - y) * canvasFrameSize * zoomLevel)};
            }

            ImVec2 mapToCanvas(Vec2f c) {
                return {float(canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel),
                        float(canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel)};
            }

            void canvasBegin() {
                // Sets the canvas display attributes
                canvasSize = ImGui::GetContentRegionAvail();
                canvasCursor = ImGui::GetCursorScreenPos();
                canvasOrigin = ImVec2(
                        canvasCursor.x + canvasSize.x / 2.0f,
                        canvasCursor.y + canvasSize.y / 2.0f
                );
                canvasMargin = ImVec2(100.0f, 100.0f);
                canvasFrameSize = std::max(
                        std::min(canvasSize.x - canvasMargin.x * 2.0f, canvasSize.y - canvasMargin.y * 2.0f), FLT_MIN);

                ImDrawList* drawList = ImGui::GetWindowDrawList();
                drawList->PushClipRect(
                        canvasCursor,
                        {canvasCursor.x + canvasSize.x, canvasCursor.y + canvasSize.y},
                        true
                );
                // Draw border and background color
                drawList->AddRectFilled(
                        canvasCursor,
                        {canvasCursor.x + canvasSize.x, canvasCursor.y + canvasSize.y},
                        IM_COL32(0, 0, 0, 255), 6.0f, 0
                );
                // Catch mouse interactions
                ImGui::InvisibleButton("canvas", canvasSize,
                                       ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
                isHovered = ImGui::IsItemHovered();
            }

            void canvasEnd() {
                framerateCounter.resetCycle();

                ImGui::GetWindowDrawList()->PopClipRect();
                ImGui::End();
            }

            void autoAdjustView() {
                double x_max = -1000000000.0;
                double x_min = 1000000000.0;
                double y_max = -1000000000.0;
                double y_min = 1000000000.0;

                ::gph::VertexIter it(Graphene::core->getGraphObj());
                while (it.next()) {
                    x_max = std::max(x_max, it.v->getCoord().x);
                    x_min = std::min(x_min, it.v->getCoord().x);
                    y_max = std::max(y_max, it.v->getCoord().y);
                    y_min = std::min(y_min, it.v->getCoord().y);
                }

                //if (rightMouseDownVertex == nullptr) {
                if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    View::centerContext.x += ((x_max - x_min) / 2.0f + x_min - View::centerContext.x) * 0.2;
                    View::centerContext.y += ((y_max - y_min) / 2.0f + y_min - View::centerContext.y) * 0.2;

                    View::zoomTarget += (1.0f / std::max(std::max(x_max - x_min, y_max - y_min), 0.1) - View::zoomLevel) * 0.2;
                }
            }

            void updateCamera() {
                if (isHovered) {
                    View::zoomTarget *= powf(1.05, ImGui::GetIO().MouseWheel);

                    if (Controls::rightMouseDownVertex == nullptr) {
                        if (!Controls::contextMenuOpen && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
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
                View::zoomLevel *= powf(View::zoomTarget / View::zoomLevel, 0.1);
                View::zoomLevel = std::max(View::zoomTarget, DBL_MIN);
            }

            void updateMouseCursor() {
                if (View::isHovered) {
                    if (Controls::hoveredVertex != nullptr || Controls::leftMouseDownVertex != nullptr || Controls::rightMouseDownVertex != nullptr)
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                    else
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                }
            }

        }
    }
}
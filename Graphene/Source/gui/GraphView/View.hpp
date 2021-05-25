#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "../../graphene/Structure.hpp"
#include "../../utils/ProfilerUtils.hpp"
#include "Common.hpp"

namespace Gui {
    namespace GraphView {
        namespace View {

            float mapToContext(float l) {
                return l / canvasFrameSize / zoomLevel;
            }

            float mapToCanvas(float l) {
                return l * canvasFrameSize * zoomLevel;
            }

            /*::Graphene::Vec2f mapToContext(ImVec2 c) {
                return {canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel,
                        canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel};
            }*/

            ImVec2 mapToCanvas(::Graphene::Vec2f c) {
                return {canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel,
                        canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel};
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
                isHovered = ImGui::IsItemFocused();
            }

            void canvasEnd() {
                framerateCounter.countFrame();

                ImGui::GetWindowDrawList()->PopClipRect();
                ImGui::End();
            }

        }
    }
}
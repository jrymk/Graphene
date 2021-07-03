#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Common.hpp"

namespace gph {
    namespace GraphView {
        namespace Overlay {

            void draw() {

                ImGui::GetWindowDrawList()->AddText(
                        ImVec2(View::canvasCursor.x + 5.0f, View::canvasCursor.y + 5.0f),
                        ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.80, 0.81, 1.0f)),
                        "Framerate", 0
                );
                ImGui::GetWindowDrawList()->AddText(
                        ImVec2(View::canvasCursor.x + 5.0f, View::canvasCursor.y + 15.0f),
                        ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.80, 0.81, 1.0f)),
                        std::to_string(View::framerateCounter.getFramerate()).c_str(), 0
                );

                ImGui::GetWindowDrawList()->AddText(
                        ImVec2(View::canvasCursor.x + 5.0f, View::canvasCursor.y + 40.0f),
                        ImGui::ColorConvertFloat4ToU32(ImVec4(0.81, 0.72, 0.59, 1.0f)),
                        "Graph update rate", 0
                );
                ImGui::GetWindowDrawList()->AddText(
                        ImVec2(View::canvasCursor.x + 5.0f, View::canvasCursor.y + 50.0f),
                        ImGui::ColorConvertFloat4ToU32(ImVec4(0.81, 0.72, 0.59, 1.0f)),
                        std::to_string(Graphene::core->updateRateCounter.getFramerate()).c_str(), 0
                );
            }

        }

    }
}
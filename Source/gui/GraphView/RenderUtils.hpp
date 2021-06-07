#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../../utils/ProfilerUtils.hpp"

#include "Common.hpp"

namespace Gui {
    namespace GraphView {
        namespace RenderUtils {

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


        }
    }
}
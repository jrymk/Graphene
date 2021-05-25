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
/*
            void drawGridLines(bool vertical, float snap, float spacing, float begin, float end, ImColor col, float thickness) {
                const float centerSnapGridCoord =
                        centerDrawCoord.x - (fmodf(centerMapped.x, contextGridSpacing) * canvasDisplaySize * zoomLevel);
                const float startingSnapGridCoord =
                        centerSnapGridCoord - (int) (drawSize.x / spacing / 2.0f + 2.0f) * spacing;
                const float endingSnapGridCoord =
                        centerSnapGridCoord + (int) (drawSize.x / spacing / 2.0f + 2.0f) * spacing;

                int lineDrawLimit = 500;
                for (int x = 0; startingSnapGridCoord + x * spacing <= endingSnapGridCoord; x++) {
                    ImGui::GetWindowDrawList()->AddLine(
                            ImVec2(startingSnapGridCoord + x * spacing, begin),
                            ImVec2(startingSnapGridCoord + x * spacing, end),
                            col,
                            thickness
                    );
                    if (!(--lineDrawLimit))
                        break;
                }
            }
*/


        }
    }
}
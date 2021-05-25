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

                ImDrawList* drawList = ImGui::GetWindowDrawList();

                /*RenderUtils::drawGridLines(
                        true,
                        View::canvasCursor.x - (fmodf(View::centerContext.x, powf(10, -(int) log10f(View::View::zoomLevel) - 1)) * View::View::canvasFrameSize * View::View::zoomLevel),
                        View::mapToCanvas(powf(10, -(int) log10f(View::View::zoomLevel) - 1)),
                        0.0f,
                        View::canvasSize.y
                );*/
                {
                    // vertical lines (secondary)
                    const float contextGridSpacing = powf(10, -(int) log10f(View::zoomLevel) - 1);
                    const float screenGridSpacing = contextGridSpacing * View::canvasFrameSize * View::zoomLevel;
                    const float centerSnapGridCoord =
                            View::canvasOrigin.x - (fmodf(View::centerContext.x, contextGridSpacing) * View::canvasFrameSize * View::zoomLevel);
                    const float startingSnapGridCoord =
                            centerSnapGridCoord - (int) (View::canvasSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
                    const float endingSnapGridCoord =
                            centerSnapGridCoord + (int) (View::canvasSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

                    int lineDrawLimit = 500;
                    for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
                        drawList->AddLine(ImVec2(x, View::canvasOrigin.y - View::canvasSize.y / 2.0f),
                                          ImVec2(x, View::canvasOrigin.y + View::canvasSize.y / 2.0f),
                                          IM_COL32(90, 90, 90, 80), 1.0f);
                        if (!(--lineDrawLimit))
                            break;
                    }
                }
                {
                    // vertical lines (primary)
                    const float contextGridSpacing = powf(10, -(int) log10f(View::zoomLevel));
                    const float screenGridSpacing = contextGridSpacing * View::canvasFrameSize * View::zoomLevel;
                    const float centerSnapGridCoord =
                            View::canvasOrigin.x - (fmodf(View::centerContext.x, contextGridSpacing) * View::canvasFrameSize * View::zoomLevel);
                    const float startingSnapGridCoord =
                            centerSnapGridCoord - (int) (View::canvasSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
                    const float endingSnapGridCoord =
                            centerSnapGridCoord + (int) (View::canvasSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

                    int lineDrawLimit = 500;
                    for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
                        drawList->AddLine(ImVec2(x, View::canvasOrigin.y - View::canvasSize.y / 2.0f),
                                          ImVec2(x, View::canvasOrigin.y + View::canvasSize.y / 2.0f),
                                          IM_COL32(90, 90, 90, 80), 2.0f);
                        if (!(--lineDrawLimit))
                            break;
                    }
                }
                {
                    // horizontal lines (secondary)
                    const float contextGridSpacing = powf(10, -(int) log10f(View::zoomLevel) - 1);
                    const float screenGridSpacing = contextGridSpacing * View::canvasFrameSize * View::zoomLevel;
                    const float centerSnapGridCoord =
                            View::canvasOrigin.y + (fmodf(View::centerContext.y, contextGridSpacing) * View::canvasFrameSize * View::zoomLevel);
                    const float startingSnapGridCoord =
                            centerSnapGridCoord - (int) (View::canvasSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
                    const float endingSnapGridCoord =
                            centerSnapGridCoord + (int) (View::canvasSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

                    int lineDrawLimit = 500;
                    for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
                        drawList->AddLine(ImVec2(View::canvasOrigin.x - View::canvasSize.x / 2.0f, y),
                                          ImVec2(View::canvasOrigin.x + View::canvasSize.x / 2.0f, y),
                                          IM_COL32(90, 90, 90, 80), 1.0f);
                        if (!(--lineDrawLimit))
                            break;
                    }
                }
                {
                    // horizontal lines (primary)
                    const float contextGridSpacing = powf(10, -(int) log10f(View::zoomLevel));
                    const float screenGridSpacing = contextGridSpacing * View::canvasFrameSize * View::zoomLevel;
                    const float centerSnapGridCoord =
                            View::canvasOrigin.y + (fmodf(View::centerContext.y, contextGridSpacing) * View::canvasFrameSize * View::zoomLevel);
                    const float startingSnapGridCoord =
                            centerSnapGridCoord - (int) (View::canvasSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
                    const float endingSnapGridCoord =
                            centerSnapGridCoord + (int) (View::canvasSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

                    int lineDrawLimit = 500;
                    for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
                        drawList->AddLine(ImVec2(View::canvasOrigin.x - View::canvasSize.x / 2.0f, y),
                                          ImVec2(View::canvasOrigin.x + View::canvasSize.x / 2.0f, y),
                                          IM_COL32(90, 90, 90, 80), 2.0f);
                        if (!(--lineDrawLimit))
                            break;
                    }
                }


                ImVec2 contextOriginCenterDelta = ImVec2(View::canvasOrigin.x - View::centerContext.x * View::canvasFrameSize * View::zoomLevel,
                                                         View::canvasOrigin.y +
                                                         View::centerContext.y * View::canvasFrameSize * View::zoomLevel);

            }

        }
    }
}
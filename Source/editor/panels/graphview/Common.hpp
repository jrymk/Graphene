#pragma once

#include "../../../core/Core.hpp"
#include "../../../tools/Profiler.hpp"
#include "../../../editor/graphic/DoubleBuffer.hpp"

namespace graphene::editor::graphview {
namespace view {

graphene::tools::profiler::CycleMonitor framerateCounter;

ImVec2 canvasSize;
ImVec2 canvasCursor;
ImVec2 canvasOrigin;
ImVec2 canvasMargin;
double canvasFrameSize;
bool isHovered = false;

ImVec2 centerContext(0.0f, 0.0f);
double zoomLevel = 1.0f;
double zoomTarget = 1.0f;
} // namespace view
namespace controls {
bool enableRender = true;
bool enableAutoAdjustView = true;
bool gridVisible = true;

bool contextMenuOpen = false;
bool forceOpenContextMenu = false;

graphene::core::Vertex* leftMouseSelectionVertex = nullptr;
graphene::core::Vertex* leftMouseDownVertex = nullptr;
ImVec2 leftMouseDownPos;
graphene::core::Vertex* rightMouseSelectionVertex = nullptr;
graphene::core::Vertex* rightMouseDownVertex = nullptr;
ImVec2 rightMouseDownPos;
graphene::core::Vertex* hoveredVertex = nullptr;
graphene::core::ConnectedComponent* hoveredComponent = nullptr;
} // namespace controls

graphene::tools::profiler::Timer* animationTimer = new graphene::tools::profiler::Timer();
} // namespace graphene::editor::graphview
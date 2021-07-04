#pragma once

#include "../../../../core/Include.h"

namespace graphene {
namespace GraphView {
namespace Graphene {
::graphene::core::Core *core;
}
namespace View {
Utils::CycleMonitor framerateCounter;

ImVec2 canvasSize;
ImVec2 canvasCursor;
ImVec2 canvasOrigin;
ImVec2 canvasMargin;
double canvasFrameSize;
bool isHovered = false;

ImVec2 centerContext(0.0f, 0.0f);
double zoomLevel = 1.0f;
double zoomTarget = 1.0f;
} // namespace View
namespace Controls {
bool enableRender = true;
bool enableAutoAdjustView = true;
bool gridVisible = true;

bool contextMenuOpen = false;
bool forceOpenContextMenu = false;

::graphene::Vertex *leftMouseSelectionVertex = nullptr;
::graphene::Vertex *leftMouseDownVertex = nullptr;
ImVec2 leftMouseDownPos;
::graphene::Vertex *rightMouseSelectionVertex = nullptr;
::graphene::Vertex *rightMouseDownVertex = nullptr;
ImVec2 rightMouseDownPos;
::graphene::Vertex *hoveredVertex = nullptr;
::graphene::ConnectedComponent *hoveredComponent = nullptr;
} // namespace Controls

Utils::Timer *animationTimer = new Utils::Timer();
} // namespace GraphView
} // namespace graphene
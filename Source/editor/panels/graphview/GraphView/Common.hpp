#pragma once

#include "../../../../core/Include.h"

namespace gph {
    namespace GraphView {
        namespace Graphene {
            ::gph::Core* core;
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
        }
        namespace Controls {
            bool enableRender = true;
            bool enableAutoAdjustView = true;
            bool gridVisible = true;

            bool contextMenuOpen = false;
            bool forceOpenContextMenu = false;

            ::gph::Vertex* leftMouseSelectionVertex = nullptr;
            ::gph::Vertex* leftMouseDownVertex = nullptr;
            ImVec2 leftMouseDownPos;
            ::gph::Vertex* rightMouseSelectionVertex = nullptr;
            ::gph::Vertex* rightMouseDownVertex = nullptr;
            ImVec2 rightMouseDownPos;
            ::gph::Vertex* hoveredVertex = nullptr;
            ::gph::ConnectedComponent* hoveredComponent = nullptr;
        }

        Utils::Timer* animationTimer = new Utils::Timer();
    }
}
#pragma once

#include "../../graphene/Include.h"
#include "../../utils/ProfilerUtils.hpp"


namespace Gui {
    namespace GraphView {
        namespace Graphene {
            ::Graphene::Core* core;
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

            ::Graphene::Vertex* leftMouseSelectionVertex = nullptr;
            ::Graphene::Vertex* leftMouseDownVertex = nullptr;
            ImVec2 leftMouseDownPos;
            ::Graphene::Vertex* rightMouseSelectionVertex = nullptr;
            ::Graphene::Vertex* rightMouseDownVertex = nullptr;
            ImVec2 rightMouseDownPos;
            ::Graphene::Vertex* hoveredVertex = nullptr;
            ::Graphene::ConnectedComponent* hoveredComponent = nullptr;
        }

        Utils::Timer* animationTimer = new Utils::Timer();
    }
}
#pragma once

#include "../../graphene/Include.h"
#include "../../utils/ProfilerUtils.hpp"


namespace Gui {
    namespace GraphView {
        namespace Graphene {
            ::Graphene::Core* core;
            ::Graphene::Graph* graph;
        }
        namespace View {
            Utils::FramerateCounter framerateCounter;

            ImVec2 canvasSize;
            ImVec2 canvasCursor;
            ImVec2 canvasOrigin;
            ImVec2 canvasMargin;
            float canvasFrameSize;
            bool isHovered = false;

            ImVec2 centerContext(0.0f, 0.0f);
            float zoomLevel = 1.0f;
            float zoomTarget = 1.0f;
        }

    }
}
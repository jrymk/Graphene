#pragma once

#include <imgui.h>
#include <Interface/Interface.hpp>
#include <Document/GraphView/Camera.hpp>

namespace gfn::editor::graphview {
    class Renderer {
    public:
        gfn::editor::graphview::Camera* camera;
        gfn::interface::Interface* interface;

        void drawEdges() {
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = interface->userprops.getRead();
            for (auto& ei : userprops->getEdgePropList()) {
                auto e = ei.second;
                if (!e.enabled)
                    continue;
                drawList->AddLine(camera->map(e.startVertexPosition), camera->map(e.endVertexPosition),
                                  e.edgeFillColor.color32, camera->map(e.thickness));
                /*drawList->AddBezierQuadratic(camera->map(e.startVertexPosition), camera->map(e.position),
                                             camera->map(e.endVertexPosition), e.edgeFillColor.color32,
                                             camera->map(e.thickness));*/

                //drawList->AddCircleFilled(camera->map(e.position), camera->map(e.radius), e.edgeNodeColor.color32, 0);
            }
        }

        void drawVertices() {
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = interface->userprops.getRead();
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
                if (!v.enabled)
                    continue;
                /*drawList->AddCircleFilled(camera->map(v.position), camera->map(v.radiusOuter), IM_COL32(0, 0, 0, 255),
                                          0);*/
                drawList->AddCircleFilled(camera->map(v.position), camera->map(v.radius), v.vertexFillColor.color32, 0);
            }
        }
    };
}
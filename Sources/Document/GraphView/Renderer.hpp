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
                drawList->AddLine(camera->map(e.startVertexPosition),
                                  camera->map(e.endVertexPosition),
                                  e.edgeFillColor.color32, camera->map(e.thickness));
            }
        }

        void drawVertices() {
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = interface->userprops.getRead();
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
                drawList->AddCircleFilled(camera->map(v.position), camera->map(v.radius), v.vertexFillColor.color32, 0);
            }
        }
    };
}
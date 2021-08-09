#pragma once

#include <imgui.h>
#include <Core/Interface/Interface.h>
#include <Editor/Document/GraphView/Camera.h>
#include <Tracy.hpp>

namespace gfn {
    class Renderer {
    private:
        gfn::Interface* itf;
        Camera* camera;
        Selection* selection;

    public:
        Renderer(gfn::Interface* itf, Camera* camera, Selection* selection) :
                itf(itf),
                camera(camera),
                selection(selection) {

        }

        void drawEdges() {
            ZoneScoped
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& ei : userprops.getEdgePropsList()) {
                auto e = ei.second;
                if (!e.enabled.value)
                    continue;
                drawList->AddLine(camera->map(e.startVertexPosition.value), camera->map(e.endVertexPosition.value),
                                  e.edgeFillColor.value, camera->map(e.thickness.value));
                /*drawList->AddBezierQuadratic(camera->map(e.startVertexPosition), camera->map(e.position),
                                             camera->map(e.endVertexPosition), e.edgeFillColor.color32,
                                             camera->map(e.thickness));*/

                //drawList->AddCircleFilled(camera->map(e.position), camera->map(e.radius), e.edgeNodeColor.color32, 0);
            }
        }

        void drawVertices() {
            ZoneScoped
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = itf->graph.getRead()->props;
            for (auto& vi : userprops.getVertexPropsList()) {
                auto v = vi.second;
                if (!v.enabled.value)
                    continue;
                /*drawList->AddCircleFilled(camera->map(v.position), camera->map(v.radiusOuter), IM_COL32(0, 0, 0, 255),
                                          0);*/
                drawList->AddCircleFilled(camera->map(v.position.value), camera->map(v.radius.value),
                                          v.vertexFillColor.value, 0);
            }
        }
    };
}
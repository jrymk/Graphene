#pragma once

#include <cmath>
#include <Interface/Interface.hpp>
#include <Document/GraphView/Camera.hpp>

namespace gfn::editor::graphview {
    class Selection {
    public:
        gfn::interface::Interface* interface;
        gfn::preferences::Preferences* preferences;
        Camera* camera;

        // these can be both active, usually we prioritize hoveredVertex, if it is null, take edge
        gfn::Uuid hoveredVertex;
        gfn::Uuid hoveredEdge;
        gfn::Uuid leftMouseDownVertex;
        gfn::Uuid leftMouseDownEdge;
        gfn::Vec2f leftMousePosDelta;
        gfn::Uuid rightMouseDownVertex;
        gfn::Uuid rightMouseDownEdge;
        gfn::Vec2f rightMousePosDelta;
        gfn::Uuid middleMouseDownVertex;
        gfn::Uuid middleMouseDownEdge;
        gfn::Vec2f middleMousePosDelta;
        gfn::Uuid leftMouseUpVertex;
        gfn::Uuid leftMouseUpEdge;
        gfn::Uuid rightMouseUpVertex;
        gfn::Uuid rightMouseUpEdge;
        gfn::Uuid middleMouseUpVertex;
        gfn::Uuid middleMouseUpEdge;
        gfn::Vec2f cursorCoord;

        float distance(ImVec2 a, ImVec2 b) {
            float deltaX = b.x - a.x;
            float deltaY = b.y - a.y;
            return sqrt(deltaX * deltaX + deltaY * deltaY);
        }

        float distanceToALine(ImVec2 p, ImVec2 a, ImVec2 b) {
            ImVec2 dp(p.x - a.x, p.y - a.y);
            ImVec2 dl(b.x - a.x, b.y - a.y);
            float dot = dp.x * dl.x + dp.y * dl.y;
            float len = dl.x * dl.x + dl.y * dl.y;
            float param = -1;
            if (len != 0)
                param = dot / len;
            ImVec2 dd;
            if (param < 0)
                dd = a;
            else if (param > 1)
                dd = b;
            else
                dd = ImVec2(a.x + param * dl.x, a.y + param * dl.y);
            return distance(p, dd);
        }

        void update() {
            hoveredVertex = "";
            hoveredEdge = "";

            auto io = ImGui::GetIO();
            auto userprops = interface->userprops.getRead();

            // finds the vertex closest to the mouse cursor and meet the distance requirement
            float minDistance = FLT_MAX;
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
                if (!v.enabled)
                    continue;
                ImVec2 center = camera->map(v.position);
                float cursorDistance = distance(io.MousePos, center);
                if (cursorDistance <= camera->map(v.radius) + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredVertex = v.uuid;
                }
            }
            // finds the edge closest to the mouse cursor and meet the distance requirement
            minDistance = FLT_MAX;
            for (auto& ei : userprops->getEdgePropList()) {
                auto e = ei.second;
                if (!e.enabled)
                    continue;
                float cursorDistance = distanceToALine(io.MousePos, camera->map(e.startVertexPosition),
                                                       camera->map(e.endVertexPosition));
                if (cursorDistance <= camera->map(e.thickness) / 2.0f + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredEdge = e.edgeUuid;
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                leftMouseDownVertex = hoveredVertex;
                leftMouseDownEdge = hoveredEdge;
                leftMousePosDelta = gfn::Vec2f(0.0, 0.0);
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                rightMouseDownVertex = hoveredVertex;
                rightMouseDownEdge = hoveredEdge;
                rightMousePosDelta = gfn::Vec2f(0.0, 0.0);
            }
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                middleMouseDownVertex = hoveredVertex;
                middleMouseDownEdge = hoveredEdge;
                middleMousePosDelta = gfn::Vec2f(0.0, 0.0);
            }
            if (!leftMouseDownVertex.empty())
                leftMousePosDelta = gfn::Vec2f(camera->rMap(io.MouseDelta.x), camera->rMap(io.MouseDelta.y));
            if (!rightMouseDownVertex.empty())
                rightMousePosDelta = gfn::Vec2f(camera->rMap(io.MouseDelta.x), camera->rMap(io.MouseDelta.y));
            if (!middleMouseDownVertex.empty())
                middleMousePosDelta = gfn::Vec2f(camera->rMap(io.MouseDelta.x), camera->rMap(io.MouseDelta.y));

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                leftMouseUpVertex = hoveredVertex;
                leftMouseUpEdge = hoveredEdge;
            } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                leftMouseDownVertex = "";
                leftMouseDownEdge = "";
                leftMouseUpVertex = "";
                leftMouseUpEdge = "";
            }
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
                rightMouseUpVertex = hoveredVertex;
                rightMouseUpEdge = hoveredEdge;
            } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                rightMouseDownVertex = "";
                rightMouseDownEdge = "";
                rightMouseUpVertex = "";
                rightMouseUpEdge = "";
            }
            if (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)) {
                middleMouseUpVertex = hoveredVertex;
                middleMouseUpEdge = hoveredEdge;
            } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
                middleMouseDownVertex = "";
                middleMouseDownEdge = "";
                middleMouseUpVertex = "";
                middleMouseUpEdge = "";
            }

            cursorCoord = camera->rMap(ImGui::GetMousePos());
        }
    };
}
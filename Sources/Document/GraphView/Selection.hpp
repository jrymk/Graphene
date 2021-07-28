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

        gfn::Uuid hoveredVertex;
        gfn::Uuid hoveredEdge;

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
            hoveredVertex = gfn::uuid::createNil();
            hoveredEdge = gfn::uuid::createNil();

            auto io = ImGui::GetIO();
            auto userprops = interface->userprops.getRead();

            // finds the vertex closest to the mouse cursor and meet the distance requirement
            float minDistance = FLT_MAX;
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
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
                auto sv = userprops->getVertexProps(e.startVertexUuid)->position;
                auto ev = userprops->getVertexProps(e.endVertexUuid)->position;
                float cursorDistance = distanceToALine(io.MousePos, camera->map(sv), camera->map(ev));
                if (cursorDistance <= e.thickness / 2.0f + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredEdge = e.edgeUuid;
                }
            }
        }
    };
}
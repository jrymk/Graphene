#pragma once

#include <cmath>
#include <Interface/Interface.hpp>
#include <Document/GraphView/Camera.hpp>
#include <earcut.hpp>

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

        std::unordered_set<gfn::Uuid> selectedVertices;
        std::unordered_set<gfn::Uuid> selectedEdges;
        std::vector<std::vector<std::pair<double, double>>> lassoSelectionVertices;
        std::vector<int> lassoSelectionIndices;

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
            auto userprops = interface->properties.getRead();

            // finds the vertex closest to the mouse cursor and meet the distance requirement
            float minDistance = FLT_MAX;
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
                if (!v.enabled.value)
                    continue;
                ImVec2 center = camera->map(v.position.value);
                float cursorDistance = distance(io.MousePos, center);
                if (cursorDistance <= camera->map(v.radius.value) + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredVertex = v.uuid.value;
                }
            }
            // finds the edge closest to the mouse cursor and meet the distance requirement
            minDistance = FLT_MAX;
            for (auto& ei : userprops->getEdgePropList()) {
                auto e = ei.second;
                if (!e.enabled.value)
                    continue;
                float cursorDistance = distanceToALine(io.MousePos, camera->map(e.startVertexPosition.value),
                                                       camera->map(e.endVertexPosition.value));
                if (cursorDistance <=
                    camera->map(e.thickness.value) / 2.0f + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredEdge = e.edgeUuid.value;
                }
            }

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                leftMouseDownVertex = hoveredVertex;
                leftMouseDownEdge = hoveredEdge;
            }
            leftMousePosDelta = gfn::Vec2f(0.0, 0.0);
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                rightMouseDownVertex = hoveredVertex;
                rightMouseDownEdge = hoveredEdge;
            }
            rightMousePosDelta = gfn::Vec2f(0.0, 0.0);
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
                middleMouseDownVertex = hoveredVertex;
                middleMouseDownEdge = hoveredEdge;
            }
            middleMousePosDelta = gfn::Vec2f(0.0, 0.0);
            if (!leftMouseDownVertex.empty())
                leftMousePosDelta += gfn::Vec2f(camera->rMap(io.MouseDelta.x), -camera->rMap(io.MouseDelta.y));
            if (!rightMouseDownVertex.empty())
                rightMousePosDelta += gfn::Vec2f(camera->rMap(io.MouseDelta.x), -camera->rMap(io.MouseDelta.y));
            if (!middleMouseDownVertex.empty())
                middleMousePosDelta += gfn::Vec2f(camera->rMap(io.MouseDelta.x), -camera->rMap(io.MouseDelta.y));

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

        void updateLassoSelection() {
            if (ImGui::GetIO().MouseClicked[ImGuiMouseButton_Left] && hoveredVertex.empty()) {
                lassoSelectionVertices.clear();
                selectedVertices.clear();
                selectedEdges.clear();
                lassoSelectionVertices.emplace_back(std::vector<std::pair<double, double>>());
            }
            if (leftMouseDownVertex.empty() && ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
                lassoSelectionVertices[lassoSelectionVertices.size() - 1].emplace_back(cursorCoord.x, cursorCoord.y);
            }

            if (!lassoSelectionVertices.empty() && ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
                selectedVertices.clear();
                lassoSelectionIndices = mapbox::earcut<int>(lassoSelectionVertices);
                for (int i = 0; i < lassoSelectionIndices.size(); i += 3) {
                    auto p1 = gfn::Vec2f(lassoSelectionVertices[0][lassoSelectionIndices[i]].first,
                                         lassoSelectionVertices[0][lassoSelectionIndices[i]].second);
                    auto p2 = gfn::Vec2f(lassoSelectionVertices[0][lassoSelectionIndices[i + 1]].first,
                                         lassoSelectionVertices[0][lassoSelectionIndices[i + 1]].second);
                    auto p3 = gfn::Vec2f(lassoSelectionVertices[0][lassoSelectionIndices[i + 2]].first,
                                         lassoSelectionVertices[0][lassoSelectionIndices[i + 2]].second);
                    bool in = false;
                    for (auto& v : interface->properties.getRead()->getVertexPropList()) {
                        auto p = v.second.position.get();
                        double d1 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
                        double d2 = (p.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p.y - p3.y);
                        double d3 = (p.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p.y - p1.y);
                        if (!(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)))) {
                            selectedVertices.insert(v.first);
                            in = true;
                        }
                    }
                    ImGui::GetWindowDrawList()->AddTriangleFilled(camera->map(p1), camera->map(p2), camera->map(p3),
                                                                  IM_COL32(3, 223, 252, 50));
                }
            }
        }
    };
}
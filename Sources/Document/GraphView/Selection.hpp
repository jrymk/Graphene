#pragma once

#include <cmath>
#include <Interface/Interface.hpp>
#include <Document/GraphView/Camera.hpp>
#include <Editor/HotKey.hpp>
#include <earcut.hpp>

namespace gfn::graphview {
    class Selection {
    public:
        gfn::interface::Interface* interface;
        gfn::preferences::Preferences* preferences;
        Camera* camera;

        int selectBtn = ImGuiMouseButton_Left;

        // these can be both active, usually we prioritize hoveredVertex, if it is null, take edge
        gfn::Vec2f mouseCoord;
        gfn::Vec2f mouseDelta;
        gfn::Vec2f _prevMouseCoord;

        gfn::Uuid hoveredVertex;
        gfn::Uuid hoveredEdge;

        // mouse states
        bool mouseOnClick[5];
        bool mouseDown[5];
        bool mouseOnRelease[5];
        bool mouseOnDoubleClick[5];

        gfn::Uuid mouseClickVertex[5];
        gfn::Uuid mouseDownVertex[5];
        gfn::Uuid mouseReleaseVertex[5];
        gfn::Uuid mouseDoubleClickVertex[5];
        gfn::Uuid mouseOnClickVertex[5];
        gfn::Uuid mouseOnReleaseVertex[5];
        gfn::Uuid mouseOnDoubleClickVertex[5];

        gfn::Uuid mouseClickEdge[5];
        gfn::Uuid mouseDownEdge[5];
        gfn::Uuid mouseReleaseEdge[5];
        gfn::Uuid mouseDoubleClickEdge[5];
        gfn::Uuid mouseOnClickEdge[5];
        gfn::Uuid mouseOnReleaseEdge[5];
        gfn::Uuid mouseOnDoubleClickEdge[5];

        std::unordered_set<gfn::Uuid> vertexSelection;
        std::unordered_set<gfn::Uuid> edgeSelection;

        // 0: subtract   1: invert   2: union
        bool lassoMultiSelectInvert = 1;

        // output states
        bool moveStarted = false;
        bool moving = false;
        bool moveEnded = false;


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
            updateMouse();
            updateLassoSelection();
            updateClickSelection();

            ImGui::Begin("Selection debugger");

            ImGui::Text("Lasso multi select mode");
            ImGui::Checkbox("INVERT", &lassoMultiSelectInvert);

            ImGui::Separator();
            ImGui::Text("LEFT");
            ImGui::Text(("mouse click: " + mouseClickVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse down: " + mouseDownVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse release: " + mouseReleaseVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse double click: " + mouseDoubleClickVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse on click: " + mouseOnClickVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse on release: " + mouseOnReleaseVertex[ImGuiMouseButton_Left]).c_str());
            ImGui::Text(("mouse on double click: " + mouseOnDoubleClickVertex[ImGuiMouseButton_Left]).c_str());

            ImGui::Separator();
            if (_lassoSelecting)
                ImGui::Text("lasso selecting");
            if (_moving)
                ImGui::Text("moving");

            ImGui::Separator();
            ImGui::Text("VERTEX SELECTION");
            for (auto& v : vertexSelection)
                ImGui::Text(v.c_str());

            ImGui::Separator();
            ImGui::Text("EDGE SELECTION");
            for (auto& e : edgeSelection)
                ImGui::Text(e.c_str());
            ImGui::End();
        }


        /// MOUSE UPDATE
        void updateMouse() {
            // update mouse related variables, for example which vertex is hovered? which one is clicked?
            // map the mouse position to the actual position for the vertices and edges
            mouseCoord = camera->rMap(ImGui::GetMousePos());
            auto userprops = interface->properties.getRead();

            hoveredVertex.clear();
            hoveredEdge.clear();
            // finds the vertex closest to the mouse cursor and meet the distance requirement
            float minDistance = FLT_MAX;
            for (auto& vi : userprops->getVertexPropList()) {
                auto v = vi.second;
                if (!v.enabled.value)
                    continue;
                ImVec2 center = camera->map(v.position.value);
                float cursorDistance = distance(ImGui::GetMousePos(), center);
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
                float cursorDistance = distanceToALine(ImGui::GetMousePos(), camera->map(e.startVertexPosition.value),
                                                       camera->map(e.endVertexPosition.value));
                if (cursorDistance <=
                    camera->map(e.thickness.value) / 2.0f + preferences->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredEdge = e.edgeUuid.value;
                }
            }

            for (int i = 0; i < ImGuiMouseButton_COUNT; i++) {
                mouseOnClickVertex[i].clear();
                mouseOnClickEdge[i].clear();
                mouseOnReleaseVertex[i].clear();
                mouseOnReleaseEdge[i].clear();
                mouseOnDoubleClickVertex[i].clear();
                mouseOnDoubleClickVertex[i].clear();

                mouseOnClick[i] = ImGui::GetIO().MouseClicked[i];
                mouseDown[i] = ImGui::GetIO().MouseDown[i];
                mouseOnRelease[i] = ImGui::GetIO().MouseReleased[i];
                mouseOnDoubleClick[i] = ImGui::GetIO().MouseDoubleClicked[i];

                if (ImGui::GetIO().MouseClicked[i]) {
                    mouseClickVertex[i] = hoveredVertex;
                    mouseOnClickVertex[i] = hoveredVertex;
                    mouseClickEdge[i] = hoveredEdge;
                    mouseOnClickEdge[i] = hoveredEdge;
                }
                if (ImGui::GetIO().MouseDown[i]) {
                    mouseDownVertex[i] = hoveredVertex;
                    mouseDownEdge[i] = hoveredEdge;
                }
                if (ImGui::GetIO().MouseReleased[i]) {
                    mouseReleaseVertex[i] = hoveredVertex;
                    mouseOnReleaseVertex[i] = hoveredVertex;
                    mouseReleaseEdge[i] = hoveredEdge;
                    mouseOnReleaseEdge[i] = hoveredEdge;
                }
                if (ImGui::GetIO().MouseDoubleClicked[i]) {
                    mouseDoubleClickVertex[i] = hoveredVertex;
                    mouseOnDoubleClickVertex[i] = hoveredVertex;
                    mouseDoubleClickVertex[i] = hoveredEdge;
                    mouseOnDoubleClickVertex[i] = hoveredEdge;
                }
            }

            mouseDelta = mouseCoord - _prevMouseCoord;
            _prevMouseCoord = mouseCoord;
        }
        /// MOUSE UPDATE

        /// LASSO SELECTION
    private:
        bool _lassoSelecting = false;

        std::vector<std::vector<std::pair<double, double>>> _lassoVertices;
        std::vector<int> _lassoIndices;
        // to undo the selections every frame to properly perform the not operation
        std::vector<gfn::Uuid> _prevLassoVertexSelection;
        std::vector<gfn::Uuid> _prevLassoEdgeSelection;
        bool _myLassoMultiSelectInvert = lassoMultiSelectInvert;

    public:
        void updateLassoSelection() {
            // undo previous lasso selection
            if (_lassoSelecting) {
                for (auto& v : _prevLassoVertexSelection) {
                    if (vertexSelection.find(v) == vertexSelection.end()) {
                        vertexSelection.insert(v);
                    } else {
                        if (_myLassoMultiSelectInvert)
                            vertexSelection.erase(v);
                    }
                }
                for (auto& e : _prevLassoEdgeSelection) {
                    if (edgeSelection.find(e) == edgeSelection.end()) {
                        edgeSelection.insert(e);
                    } else {
                        if (_myLassoMultiSelectInvert)
                            edgeSelection.erase(e);
                    }
                }
            }
            _myLassoMultiSelectInvert = lassoMultiSelectInvert;

            // start the lasso selection
            // CRITERIA: click at empty space
            if (ImGui::IsWindowFocused() && mouseOnClick[selectBtn] && hoveredVertex.empty() && hoveredEdge.empty()) {
                _lassoVertices.clear();
                _lassoVertices.emplace_back(std::vector<std::pair<double, double>>());
                _lassoSelecting = true;
                // clear selection then starting lasso selection
                // CRITERIA: shift not held
                if (!(ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_SHIFT))) {
                    vertexSelection.clear();
                    edgeSelection.clear();
                }
            }

            if (_lassoSelecting) {
                if (!_lassoVertices[0].empty() && distance(camera->map(gfn::Vec2f(_lassoVertices[0][_lassoVertices[0].size() - 1].first,
                                                    _lassoVertices[0][_lassoVertices[0].size() - 1].second)), ImGui::GetMousePos()) < 2.0f)
                    _lassoVertices[0].pop_back();
                _lassoVertices[0].emplace_back(camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).x,
                                               camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).y);

                _prevLassoVertexSelection.clear();
                //_prevLassoEdgeSelection.clear();

                _lassoIndices = mapbox::earcut<int>(_lassoVertices);
//                for (int i = 0; i < _lassoIndices.size(); i += 3) {
//                    auto p1 = ImVec2(_lassoVertices[0][_lassoIndices[i]].first,
//                                     _lassoVertices[0][_lassoIndices[i]].second);
//                    auto p2 = ImVec2(_lassoVertices[0][_lassoIndices[i + 1]].first,
//                                     _lassoVertices[0][_lassoIndices[i + 1]].second);
//                    auto p3 = ImVec2(_lassoVertices[0][_lassoIndices[i + 2]].first,
//                                     _lassoVertices[0][_lassoIndices[i + 2]].second);
//                    for (auto& v : interface->properties.getRead()->getVertexPropList()) {
//                        // checks if vertex is in the triangle
//                        auto p1d = camera->rMap(p1);
//                        auto p2d = camera->rMap(p2);
//                        auto p3d = camera->rMap(p3);
//                        auto pd = v.second.position.get();
//                        double d1 = (pd.x - p2d.x) * (p1d.y - p2d.y) - (p1d.x - p2d.x) * (pd.y - p2d.y);
//                        double d2 = (pd.x - p3d.x) * (p2d.y - p3d.y) - (p2d.x - p3d.x) * (pd.y - p3d.y);
//                        double d3 = (pd.x - p1d.x) * (p3d.y - p1d.y) - (p3d.x - p1d.x) * (pd.y - p1d.y);
//                        if (!(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0))))
//                            _prevLassoVertexSelection.push_back(v.first);
//                    }
//                    ImGui::GetWindowDrawList()->AddTriangleFilled(p1, p2, p3,
//                                                                  IM_COL32(3, 223, 252, 50));
//                }

                for (int i = 0; i < _lassoIndices.size(); i += 3) {
                    auto p1 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i]].first,
                                         _lassoVertices[0][_lassoIndices[i]].second);
                    auto p2 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i + 1]].first,
                                         _lassoVertices[0][_lassoIndices[i + 1]].second);
                    auto p3 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i + 2]].first,
                                         _lassoVertices[0][_lassoIndices[i + 2]].second);
                    for (auto& v : interface->properties.getRead()->getVertexPropList()) {
                        // checks if vertex is in the triangle
                        auto p = v.second.position.get();
                        double d1 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
                        double d2 = (p.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p.y - p3.y);
                        double d3 = (p.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p.y - p1.y);
                        if (!(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0))))
                            _prevLassoVertexSelection.push_back(v.first);
                    }
                    ImGui::GetWindowDrawList()->AddTriangleFilled(camera->map(p1), camera->map(p2), camera->map(p3),
                                                                  IM_COL32(214, 240, 255, 255));
                }
                std::vector<ImVec2> polyVertices;
                for (auto& v : _lassoVertices[0])
                    polyVertices.emplace_back(camera->map(gfn::Vec2f(v.first, v.second)).x, camera->map(gfn::Vec2f(v.first, v.second)).y);
                ImGui::GetWindowDrawList()->AddPolyline(polyVertices.data(), polyVertices.size(), IM_COL32(112, 203, 255, 255),
                                                        ImDrawListFlags_AntiAliasedLinesUseTex, 3.0f);

                // commit to main selection
                for (auto& v : _prevLassoVertexSelection) {
                    if (vertexSelection.find(v) == vertexSelection.end()) {
                        vertexSelection.insert(v);
                    } else {
                        if (_myLassoMultiSelectInvert)
                            vertexSelection.erase(v);
                    }
                }
                for (auto& e : _prevLassoEdgeSelection) {
                    if (edgeSelection.find(e) == edgeSelection.end()) {
                        edgeSelection.insert(e);
                    } else {
                        if (_myLassoMultiSelectInvert)
                            edgeSelection.erase(e);
                    }
                }
            }

            // end lasso selection
            // CRITERIA: released mouse button
            if (!mouseDown[selectBtn])
                _lassoSelecting = false;
        }
        /// LASSO SELECTION

        /// CLICK SELECTION
    private:
        bool _moving = false;

    public:
        void updateClickSelection() {
            // click empty space to deselect all is handled by updateLassoSelection, as clicking empty space is starting a latto selection

            // click to remove all selection and select one item
            // CRITERIA: when unshifted and clicking on a deselected vertex, mouse click selects it. (clicking on a selected vertex moves it)
            //           *dragging a vertex to create an edge DOES select the vertex
            auto clickVertex = mouseOnClickVertex[selectBtn];
            if (!clickVertex.empty()) {
                if (!(ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_SHIFT))) {
                    // unshifted
                    if (vertexSelection.find(clickVertex) == vertexSelection.end()) {
                        // deselected vertex
                        vertexSelection.clear();
                        edgeSelection.clear();
                        vertexSelection.insert(clickVertex);
                    } else {
                        // selected vertex
                        _moving = true;
                    }
                } else {
                    // shifted: invert selection
                    if (vertexSelection.find(clickVertex) == vertexSelection.end())
                        vertexSelection.insert(clickVertex);
                    else
                        vertexSelection.erase(clickVertex);
                }
            }

            if (!mouseDown[selectBtn])
                _moving = false;

            moveStarted = false;
            moveEnded = false;
            if (moving && !_moving)
                moveEnded = true;
            else if (!moving && _moving)
                moveStarted = true;
            moving = _moving;
        }
        /// CLICK SELECTION
    };
}
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
        bool _hover_e = false; // empty selection
        bool _hover_u = false; // hovered on unselected item
        bool _hover_s = false; // hovered on selected item
        bool _start_hover_e = false; // empty selection
        bool _start_hover_u = false; // hovered on unselected item
        bool _start_hover_s = false; // hovered on selected item
        bool _lassoAddSelectMode;
        bool _lassoSubtractSelectMode;
        bool _lassoInvSelectMode;

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
        bool onChangeSelection = false;

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
            onChangeSelection = false;
            updateMouse();
            updateLassoSelection();
            updateClickSelection();

            ImGui::Begin("Selection debugger");

            ImGui::Text("On selection update");
            ImGui::SameLine(100.0f);
            ImGui::Text((onChangeSelection ? "true" : "false"));

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
            if (lassoSelecting)
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

            ImGui::Begin("Hot key debugger");
            if (_start_hover_e)
                ImGui::Text("start E");
            if (_start_hover_u)
                ImGui::Text("start U");
            if (_start_hover_s)
                ImGui::Text("start S");
            if (_lassoAddSelectMode)
                ImGui::Text("SEL");
            if (_lassoSubtractSelectMode)
                ImGui::Text("DESEL");
            if (_lassoInvSelectMode)
                ImGui::Text("INVSEL");

            ImGui::Separator();
            ImGui::Text("ACTIONS");
            for (int i = 0; i < gfn::keybind::actions.size(); i++) {
                if (gfn::editor::hkDown(i))
                    ImGui::Text(gfn::keybind::actions[i].c_str());
            }
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
            for (auto& vi : userprops->getVertexPropsList()) {
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
            if (hoveredVertex.empty()) {
                minDistance = FLT_MAX;
                for (auto& ei : userprops->getEdgePropsList()) {
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
            }

            for (int i = 0; i < ImGuiMouseButton_COUNT; i++) {
                mouseOnClickVertex[i].clear();
                mouseOnReleaseVertex[i].clear();
                mouseOnDoubleClickVertex[i].clear();
                mouseOnClickEdge[i].clear();
                mouseOnDoubleClickEdge[i].clear();
                mouseOnReleaseEdge[i].clear();

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

            _hover_e = hoveredVertex.empty() && hoveredEdge.empty();
            _hover_s = (!hoveredVertex.empty() && vertexSelection.find(hoveredVertex) != vertexSelection.end())
                       || (!hoveredEdge.empty() && edgeSelection.find(hoveredEdge) != edgeSelection.end());
            _hover_u = !(_hover_e || _hover_s);
        }
        /// MOUSE UPDATE

        /// LASSO SELECTION
    private:
        std::vector<std::vector<std::pair<double, double>>> _lassoVertices;
        std::vector<int> _lassoIndices;
        // to undo the selections every frame to properly perform the not operation
        std::unordered_set<gfn::Uuid> _vertexSelectionOnLasso;
        std::unordered_set<gfn::Uuid> _edgeSelectionOnLasso;

    public:
        bool lassoSelecting = false;

        void updateLassoSelection() {
            // DESELECT
            if ((_hover_e && gfn::editor::hkPress(keybind::Actions::CLEAR_SELECTION_E))
                || (_hover_u && gfn::editor::hkPress(keybind::Actions::CLEAR_SELECTION_U))
                || (_hover_s && gfn::editor::hkPress(keybind::Actions::CLEAR_SELECTION_S))) {
                vertexSelection.clear();
                edgeSelection.clear();
            }

            if (lassoSelecting) {
                // undo previous lasso selection
                onChangeSelection = true;
                vertexSelection = _vertexSelectionOnLasso;
                edgeSelection = _edgeSelectionOnLasso;
            }

            /// LASSO BEGIN
            if (!lassoSelecting && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
                && ((_hover_e && (gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_E) ||
                                  gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_E) ||
                                  gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_E)))
                    || (_hover_u && (gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_U) ||
                                     gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_U) ||
                                     gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_U)))
                    || (_hover_s && (gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_S) ||
                                     gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_S) ||
                                     gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_S))))) {
                _lassoVertices.clear();
                _lassoVertices.emplace_back(std::vector<std::pair<double, double>>());

                _start_hover_e = _hover_e;
                _start_hover_u = _hover_u;
                _start_hover_s = _hover_s;

                _lassoAddSelectMode = (_hover_e && gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_E))
                                      || (_hover_u && gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_U))
                                      || (_hover_s && gfn::editor::hkPress(keybind::Actions::ADD_SELECT_LASSO_S));
                _lassoSubtractSelectMode = (_hover_e && gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_E))
                                           || (_hover_u && gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_U))
                                           || (_hover_s && gfn::editor::hkPress(keybind::Actions::SUBTRACT_SELECT_LASSO_S));
                _lassoInvSelectMode = (_hover_e && gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_E))
                                      || (_hover_u && gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_U))
                                      || (_hover_s && gfn::editor::hkPress(keybind::Actions::INV_SELECT_LASSO_S));

                // clear selection then starting lasso selection
                // CRITERIA: shift not held
                _vertexSelectionOnLasso = vertexSelection;
                _edgeSelectionOnLasso = edgeSelection;

                lassoSelecting = true;
            }

            if (lassoSelecting) {
                if (!_lassoVertices[0].empty() && distance(
                        camera->map(gfn::Vec2f(
                                _lassoVertices[0][_lassoVertices[0].size() - 1].first,
                                _lassoVertices[0][_lassoVertices[0].size() - 1].second)), ImGui::GetMousePos()) < 2.0f)
                    _lassoVertices[0].pop_back();

                _lassoVertices[0].emplace_back(camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).x,
                                               camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).y);

                _lassoIndices = mapbox::earcut<int>(_lassoVertices);

                for (int i = 0; i < _lassoIndices.size(); i += 3) {
                    auto p1 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i]].first,
                                         _lassoVertices[0][_lassoIndices[i]].second);
                    auto p2 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i + 1]].first,
                                         _lassoVertices[0][_lassoIndices[i + 1]].second);
                    auto p3 = gfn::Vec2f(_lassoVertices[0][_lassoIndices[i + 2]].first,
                                         _lassoVertices[0][_lassoIndices[i + 2]].second);
                    for (auto& v : interface->properties.getRead()->getVertexPropsList()) {
                        // checks if vertex is in the triangle
                        auto p = v.second.position.get();
                        double d1 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
                        double d2 = (p.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p.y - p3.y);
                        double d3 = (p.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p.y - p1.y);
                        if (!(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)))) {
                            // commit to main selection
                            if (vertexSelection.find(v.first) == vertexSelection.end()) {
                                // not in selection
                                if (_lassoAddSelectMode || _lassoInvSelectMode)
                                    vertexSelection.insert(v.first);
                            } else {
                                // is in selection
                                if (_lassoSubtractSelectMode || _lassoInvSelectMode)
                                    vertexSelection.erase(v.first);
                            }
                        }
                    }
                    for (auto& e : interface->properties.getRead()->getEdgePropsList()) {
                        // checks if edge is in the triangle
                        auto p = e.second.position.get();
                        double d1 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
                        double d2 = (p.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p.y - p3.y);
                        double d3 = (p.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p.y - p1.y);
                        if (!(((d1 < 0) || (d2 < 0) || (d3 < 0)) && ((d1 > 0) || (d2 > 0) || (d3 > 0)))) {
                            if (edgeSelection.find(e.first) == edgeSelection.end()) {
                                // not in selection
                                if (_lassoAddSelectMode || _lassoInvSelectMode)
                                    edgeSelection.insert(e.first);
                            } else {
                                // is in selection
                                if (_lassoSubtractSelectMode || _lassoInvSelectMode)
                                    edgeSelection.erase(e.first);
                            }
                        }
                    }
                    ImGui::GetWindowDrawList()->AddTriangleFilled(camera->map(p1), camera->map(p2), camera->map(p3),
                                                                  IM_COL32(214, 240, 255, 255));
                }
                std::vector<ImVec2> polyVertices;
                for (auto& v : _lassoVertices[0])
                    polyVertices.emplace_back(camera->map(gfn::Vec2f(v.first, v.second)).x, camera->map(gfn::Vec2f(v.first, v.second)).y);
                ImGui::GetWindowDrawList()->AddPolyline(polyVertices.data(), polyVertices.size(), IM_COL32(112, 203, 255, 255),
                                                        ImDrawListFlags_AntiAliasedLinesUseTex, 3.0f);
            }

            // end lasso selection
            // CRITERIA: released mouse button
            if (lassoSelecting && ((_start_hover_e && !(gfn::editor::hkDown(keybind::Actions::ADD_SELECT_LASSO_E) ||
                                                        gfn::editor::hkDown(keybind::Actions::SUBTRACT_SELECT_LASSO_E) ||
                                                        gfn::editor::hkDown(keybind::Actions::INV_SELECT_LASSO_E)))
                                   || (_start_hover_u && !(gfn::editor::hkDown(keybind::Actions::ADD_SELECT_LASSO_U) ||
                                                           gfn::editor::hkDown(keybind::Actions::SUBTRACT_SELECT_LASSO_U) ||
                                                           gfn::editor::hkDown(keybind::Actions::INV_SELECT_LASSO_U)))
                                   || (_start_hover_s && !(gfn::editor::hkDown(keybind::Actions::ADD_SELECT_LASSO_S) ||
                                                           gfn::editor::hkDown(keybind::Actions::SUBTRACT_SELECT_LASSO_S) ||
                                                           gfn::editor::hkDown(keybind::Actions::INV_SELECT_LASSO_S)))))
                lassoSelecting = false;
        }
        /// LASSO SELECTION

        /// CLICK SELECTION
    private:
        bool _moving = false;

    public:
        void updateClickSelection() {

            // click to remove all selection and select one item
            // CRITERIA: when unshifted and clicking on a deselected vertex, mouse click selects it. (clicking on a selected vertex moves it)
            //           *dragging a vertex to create an edge DOES select the vertex
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
                auto clickVertex = mouseOnClickVertex[selectBtn];
                if (!clickVertex.empty()) {
                    if (!(ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_SHIFT))) {
                        // unshifted
                        if (vertexSelection.find(clickVertex) == vertexSelection.end()) {
                            // deselected vertex
                            vertexSelection.clear();
                            edgeSelection.clear();
                            vertexSelection.insert(clickVertex);
                            onChangeSelection = true;
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
                        onChangeSelection = true;
                    }
                }
                auto clickEdge = mouseOnClickEdge[selectBtn];
                if (!clickEdge.empty()) {
                    if (!(ImGui::IsKeyDown(GLFW_KEY_LEFT_SHIFT) || ImGui::IsKeyDown(GLFW_KEY_RIGHT_SHIFT))) {
                        // unshifted
                        if (edgeSelection.find(clickEdge) == edgeSelection.end()) {
                            // deselected vertex
                            vertexSelection.clear();
                            edgeSelection.clear();
                            edgeSelection.insert(clickEdge);
                            onChangeSelection = true;
                        } else {
                            // selected vertex
                            _moving = true;
                        }
                    } else {
                        // shifted: invert selection
                        if (edgeSelection.find(clickEdge) == edgeSelection.end())
                            edgeSelection.insert(clickEdge);
                        else
                            edgeSelection.erase(clickEdge);
                        onChangeSelection = true;
                    }
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
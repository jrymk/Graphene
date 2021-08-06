#pragma once

#include <cmath>
#include <Core/Interface/Interface.h>
#include <Editor/Document/GraphView/Camera.h>
#include <Editor/HotKeyHandler.h>
#include <earcut.hpp>

namespace gfn {
    class Selection {
    private:
        gfn::Interface* itf;
        Camera* camera;
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;

        int selectBtn = ImGuiMouseButton_Left;
    public:
        Selection(gfn::Interface* itf, Camera* camera, gfn::HKHandler* hk, gfn::Preferences* prefs) :
                itf(itf),
                camera(camera),
                hk(hk),
                prefs(prefs) {

        }

        // these can be both active, usually we prioritize hoveredVertex, if it is null, take edge
        gfn::Vec2 mouseCoord;
        gfn::Vec2 mouseDelta;
        gfn::Vec2 _prevMouseCoord;
        int mouseHoverState = 0;
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
            MTR_SCOPE("graphview", "update selection");

            onChangeSelection = false;
            updateMouse();
            updateClearSelection();
            updateLassoSelection();
            updateClickSelection();
            updateSelectAll();

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
            ImGui::Text(std::to_string(mouseHoverState).c_str());

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
            for (int i = 0; i < gfn::Bindings::actionNames.size(); i++) {
                if (hk->down(i))
                    ImGui::Text(gfn::Bindings::actionNames[i].c_str());
            }
            ImGui::End();
        }


        /// MOUSE UPDATE
        void updateMouse() {
            // update mouse related variables, for example which vertex is hovered? which one is clicked?
            // map the mouse position to the actual position for the vertices and edges
            mouseCoord = camera->rMap(ImGui::GetMousePos()); // (one frame later)
            auto userprops = itf->graph.getRead()->props;

            hoveredVertex.clear();
            hoveredEdge.clear();
            // finds the vertex closest to the mouse cursor and meet the distance requirement
            float minDistance = FLT_MAX;
            for (auto& vi : userprops.getVertexPropsList()) {
                auto v = vi.second;
                if (!v.enabled.value)
                    continue;
                ImVec2 center = camera->map(v.position.value);
                float cursorDistance = distance(ImGui::GetMousePos(), center);
                if (cursorDistance <= camera->map(v.radius.value) + prefs->graphview_selection_tolerance &&
                    cursorDistance < minDistance) {
                    minDistance = cursorDistance;
                    hoveredVertex = v.uuid.value;
                }
            }
            // finds the edge closest to the mouse cursor and meet the distance requirement
            if (hoveredVertex.empty()) {
                minDistance = FLT_MAX;
                for (auto& ei : userprops.getEdgePropsList()) {
                    auto e = ei.second;
                    if (!e.enabled.value)
                        continue;
                    float cursorDistance = distanceToALine(ImGui::GetMousePos(),
                                                           camera->map(e.startVertexPosition.value),
                                                           camera->map(e.endVertexPosition.value));
                    if (cursorDistance <=
                        camera->map(e.thickness.value) / 2.0f + prefs->graphview_selection_tolerance &&
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

            mouseHoverState = _hover_e * (1 << 0) | _hover_u * (1 << 1) | _hover_s * (1 << 2);
        }
        /// MOUSE UPDATE

        /// CLEAR SECTION
        void updateClearSelection() {
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
                && ((_hover_e && hk->press(Actions::SUBTRACT_SELECT_ALL_E))
                    || (_hover_u && hk->press(Actions::SUBTRACT_SELECT_ALL_U))
                    || (_hover_s && hk->press(Actions::SUBTRACT_SELECT_ALL_S)))) {
                vertexSelection.clear();
                edgeSelection.clear();
            }
        }
        /// CLEAR SECTION

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
            if (lassoSelecting) {
                // undo previous lasso selection
                onChangeSelection = true;
                vertexSelection = _vertexSelectionOnLasso;
                edgeSelection = _edgeSelectionOnLasso;
            }

            /// LASSO BEGIN
            if (!lassoSelecting && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
                && ((_hover_e && (hk->press(Actions::ADD_SELECT_LASSO_E) ||
                                  hk->press(Actions::SUBTRACT_SELECT_LASSO_E) ||
                                  hk->press(Actions::INV_SELECT_LASSO_E)))
                    || (_hover_u && (hk->press(Actions::ADD_SELECT_LASSO_U) ||
                                     hk->press(Actions::SUBTRACT_SELECT_LASSO_U) ||
                                     hk->press(Actions::INV_SELECT_LASSO_U)))
                    || (_hover_s && (hk->press(Actions::ADD_SELECT_LASSO_S) ||
                                     hk->press(Actions::SUBTRACT_SELECT_LASSO_S) ||
                                     hk->press(Actions::INV_SELECT_LASSO_S))))) {
                _lassoVertices.clear();
                _lassoVertices.emplace_back(std::vector<std::pair<double, double>>());

                _start_hover_e = _hover_e;
                _start_hover_u = _hover_u;
                _start_hover_s = _hover_s;

                _lassoAddSelectMode = (_hover_e && hk->press(Actions::ADD_SELECT_LASSO_E))
                                      || (_hover_u && hk->press(Actions::ADD_SELECT_LASSO_U))
                                      || (_hover_s && hk->press(Actions::ADD_SELECT_LASSO_S));
                _lassoSubtractSelectMode = (_hover_e && hk->press(Actions::SUBTRACT_SELECT_LASSO_E))
                                           ||
                                           (_hover_u && hk->press(Actions::SUBTRACT_SELECT_LASSO_U))
                                           || (_hover_s &&
                                               hk->press(Actions::SUBTRACT_SELECT_LASSO_S));
                _lassoInvSelectMode = (_hover_e && hk->press(Actions::INV_SELECT_LASSO_E))
                                      || (_hover_u && hk->press(Actions::INV_SELECT_LASSO_U))
                                      || (_hover_s && hk->press(Actions::INV_SELECT_LASSO_S));

                // clear selection then starting lasso selection
                // CRITERIA: shift not held
                _vertexSelectionOnLasso = vertexSelection;
                _edgeSelectionOnLasso = edgeSelection;

                lassoSelecting = true;
            }

            if (lassoSelecting) {
                if (!_lassoVertices[0].empty() && distance(
                        camera->map(gfn::Vec2(
                                _lassoVertices[0][_lassoVertices[0].size() - 1].first,
                                _lassoVertices[0][_lassoVertices[0].size() - 1].second)), ImGui::GetMousePos()) < 2.0f)
                    _lassoVertices[0].pop_back();

                _lassoVertices[0].emplace_back(camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).x,
                                               camera->rMap(ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y)).y);

                _lassoIndices = mapbox::earcut<int>(_lassoVertices);

                for (int i = 0; i < _lassoIndices.size(); i += 3) {
                    auto p1 = gfn::Vec2(_lassoVertices[0][_lassoIndices[i]].first,
                                        _lassoVertices[0][_lassoIndices[i]].second);
                    auto p2 = gfn::Vec2(_lassoVertices[0][_lassoIndices[i + 1]].first,
                                        _lassoVertices[0][_lassoIndices[i + 1]].second);
                    auto p3 = gfn::Vec2(_lassoVertices[0][_lassoIndices[i + 2]].first,
                                        _lassoVertices[0][_lassoIndices[i + 2]].second);
                    for (auto& v : itf->graph.getRead()->props.getVertexPropsList()) {
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
                    for (auto& e : itf->graph.getRead()->props.getEdgePropsList()) {
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
                    polyVertices.emplace_back(camera->map(gfn::Vec2(v.first, v.second)).x,
                                              camera->map(gfn::Vec2(v.first, v.second)).y);
                ImGui::GetWindowDrawList()->AddPolyline(polyVertices.data(), polyVertices.size(),
                                                        IM_COL32(112, 203, 255, 255),
                                                        ImDrawListFlags_AntiAliasedLinesUseTex, 3.0f);
            }

            // end lasso selection
            // CRITERIA: released mouse button
            if (lassoSelecting && ((_start_hover_e && !(hk->down(Actions::ADD_SELECT_LASSO_E) ||
                                                        hk->down(
                                                                Actions::SUBTRACT_SELECT_LASSO_E) ||
                                                        hk->down(Actions::INV_SELECT_LASSO_E)))
                                   || (_start_hover_u && !(hk->down(Actions::ADD_SELECT_LASSO_U) ||
                                                           hk->down(
                                                                   Actions::SUBTRACT_SELECT_LASSO_U) ||
                                                           hk->down(Actions::INV_SELECT_LASSO_U)))
                                   || (_start_hover_s && !(hk->down(Actions::ADD_SELECT_LASSO_S) ||
                                                           hk->down(
                                                                   Actions::SUBTRACT_SELECT_LASSO_S) ||
                                                           hk->down(Actions::INV_SELECT_LASSO_S)))))
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
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)
                && ((_hover_u && (hk->press(Actions::ADD_SELECT_SINGLE_U) ||
                                  hk->press(Actions::INV_SELECT_SINGLE_U)))
                    || (_hover_s && (hk->press(Actions::SUBTRACT_SELECT_SINGLE_S) ||
                                     hk->press(Actions::INV_SELECT_SINGLE_S))))) {
                if (!hoveredVertex.empty()) {
                    if (vertexSelection.find(hoveredVertex) == vertexSelection.end()) {
                        // not in selection
                        if (hk->press(Actions::ADD_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_S)) // ADD OR INV
                            vertexSelection.insert(hoveredVertex);
                    } else {
                        // is in selection
                        if (hk->press(Actions::SUBTRACT_SELECT_SINGLE_S) ||
                            hk->press(Actions::INV_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_S)) // SUBTRACT OR INV
                            vertexSelection.erase(hoveredVertex);
                    }
                    onChangeSelection = true;
                } else if (!hoveredEdge.empty()) {
                    if (edgeSelection.find(hoveredEdge) == edgeSelection.end()) {
                        // not in selection
                        if (hk->press(Actions::ADD_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_S)) // ADD OR INV
                            edgeSelection.insert(hoveredEdge);
                    } else {
                        // is in selection
                        if (hk->press(Actions::SUBTRACT_SELECT_SINGLE_S) ||
                            hk->press(Actions::INV_SELECT_SINGLE_U) ||
                            hk->press(Actions::INV_SELECT_SINGLE_S)) // SUBTRACT OR INV
                            edgeSelection.erase(hoveredEdge);
                    }
                    onChangeSelection = true;
                }
            }
        }
        /// CLICK SELECTION

        /// SELECT ALL
        void updateSelectAll() {
            if ((_hover_e && hk->press(Actions::ADD_SELECT_ALL_E))
                || (_hover_u && hk->press(Actions::ADD_SELECT_ALL_U))
                || (_hover_s && hk->press(Actions::ADD_SELECT_ALL_S))) {
                for (auto& v : itf->graph.getRead()->props.getVertexPropsList())
                    vertexSelection.insert(v.first);
                for (auto& e : itf->graph.getRead()->props.getEdgePropsList())
                    edgeSelection.insert(e.first);
            }
            if ((_hover_e && hk->press(Actions::INV_SELECT_ALL_E))
                || (_hover_u && hk->press(Actions::INV_SELECT_ALL_U))
                || (_hover_s && hk->press(Actions::INV_SELECT_ALL_S))) {
                for (auto& v : itf->graph.getRead()->props.getVertexPropsList()) {
                    if (vertexSelection.find(v.first) == vertexSelection.end())
                        vertexSelection.insert(v.first);
                    else
                        vertexSelection.erase(v.first);
                }
                for (auto& e : itf->graph.getRead()->props.getEdgePropsList()) {
                    if (edgeSelection.find(e.first) == edgeSelection.end())
                        edgeSelection.insert(e.first);
                    else
                        edgeSelection.erase(e.first);
                }
            }
        }
        /// SELECT ALL
    };
}
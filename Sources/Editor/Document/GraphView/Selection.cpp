#include "Selection.h"
#include <Tracy.hpp>

namespace gfn {
    Selection::Selection(gfn::Interface* itf, Camera* camera, gfn::HKHandler* hk, gfn::Preferences* prefs) :
            itf(itf),
            camera(camera),
            hk(hk),
            prefs(prefs) {
    }

    void Selection::updateHoverState() {
        ZoneScoped
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

        mouseDelta = mouseCoord - _prevMouseCoord;
        _prevMouseCoord = mouseCoord;

        bool _hover_e = hoveredVertex.empty() && hoveredEdge.empty();
        bool _hover_s = (!hoveredVertex.empty() && vertexSelection.find(hoveredVertex) != vertexSelection.end())
                        || (!hoveredEdge.empty() && edgeSelection.find(hoveredEdge) != edgeSelection.end());
        bool _hover_u = !(_hover_e || _hover_s);

        if (_hover_e)
            camera->hoverState = COND_VOID;
        else if (_hover_u && !hoveredVertex.empty())
            camera->hoverState = COND_UNSEL_VERTEX;
        else if (_hover_u && !hoveredEdge.empty())
            camera->hoverState = COND_UNSEL_EDGE;
        else if (_hover_s && !hoveredVertex.empty())
            camera->hoverState = COND_SEL_VERTEX;
        else if (_hover_s && !hoveredEdge.empty())
            camera->hoverState = COND_SEL_EDGE;

        // we all rely on camera, camera itself has no access to the hot key functions
        if (ImGui::IsItemHovered() && press(Actions::CAMERA_PAN))
            camera->_canPan = true;
        if (camera->_canPan && !down(Actions::CAMERA_PAN))
            camera->_canPan = false;

        if (ImGui::IsItemHovered() && press(Actions::ZOOM_IN)) {
            camera->_canZoomIn = true;
            camera->_zoomInVelocity = velocity(Actions::ZOOM_IN);
        }
        if (camera->_canZoomIn && !down(Actions::ZOOM_IN))
            camera->_canZoomIn = false;

        if (ImGui::IsItemHovered() && press(Actions::ZOOM_OUT)) {
            camera->_canZoomOut = true;
            camera->_zoomOutVelocity = velocity(Actions::ZOOM_OUT);
        }
        if (camera->_canZoomOut && !down(Actions::ZOOM_OUT))
            camera->_canZoomOut = false;

    }

    void Selection::updateSelection() {
        ZoneScoped

        onChangeSelection = false;
        updateClearSelection();
        updateLassoSelection();
        updateClickSelection();
        updateSelectAll();
    }

    void Selection::updateClearSelection() {
        ZoneScoped

        if (ImGui::IsItemHovered()
            && press(Actions::SUBTRACT_SELECT_ALL_VERTEX)) {
            vertexSelection.clear();
        }
        if (ImGui::IsItemHovered()
            && press(Actions::SUBTRACT_SELECT_ALL_EDGE)) {
            edgeSelection.clear();
        }
    }

    void Selection::updateLassoSelection() {
        ZoneScoped

        if (lassoSelecting) {
            // undo previous lasso selection
            onChangeSelection = true;
            vertexSelection = _vertexSelectionOnLasso;
            edgeSelection = _edgeSelectionOnLasso;
        }

        /// LASSO BEGIN
        if (!lassoSelecting && ImGui::IsItemHovered()
            && (press(Actions::ADD_SELECT_LASSO_VERTEX) ||
                press(Actions::SUBTRACT_SELECT_LASSO_VERTEX) ||
                press(Actions::INV_SELECT_LASSO_VERTEX) ||
                press(Actions::ADD_SELECT_LASSO_EDGE) ||
                press(Actions::SUBTRACT_SELECT_LASSO_EDGE) ||
                press(Actions::INV_SELECT_LASSO_EDGE))) {
            _lassoVertices.clear();
            _lassoVertices.emplace_back(std::vector<std::pair<double, double>>());

            _lassoPressState = camera->hoverState;

            _lassoAddVertexMode = press(Actions::ADD_SELECT_LASSO_VERTEX);
            _lassoSubtractVertexMode = press(Actions::SUBTRACT_SELECT_LASSO_VERTEX);
            _lassoInvVertexMode = press(Actions::INV_SELECT_LASSO_VERTEX);
            _lassoAddEdgeMode = press(Actions::ADD_SELECT_LASSO_EDGE);
            _lassoSubtractEdgeMode = press(Actions::SUBTRACT_SELECT_LASSO_EDGE);
            _lassoInvEdgeMode = press(Actions::INV_SELECT_LASSO_EDGE);

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
                            if (_lassoAddVertexMode || _lassoInvVertexMode)
                                vertexSelection.insert(v.first);
                        } else {
                            // is in selection
                            if (_lassoSubtractVertexMode || _lassoInvVertexMode)
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
                            if (_lassoAddEdgeMode || _lassoInvEdgeMode)
                                edgeSelection.insert(e.first);
                        } else {
                            // is in selection
                            if (_lassoSubtractEdgeMode || _lassoInvEdgeMode)
                                edgeSelection.erase(e.first);
                        }
                    }
                }
                ImVec2 r1 = camera->map(p1);
                ImVec2 r2 = camera->map(p2);
                ImVec2 r3 = camera->map(p3);
                ImGui::GetWindowDrawList()->AddTriangleFilled(r1, r2, r3,
                                                              IM_COL32(214, 240, 255, 255));//214, 240, 255, 50
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
        if (lassoSelecting
            && !(hk->down(Actions::ADD_SELECT_LASSO_VERTEX, _lassoPressState) ||
                 hk->down(Actions::SUBTRACT_SELECT_LASSO_VERTEX, _lassoPressState) ||
                 hk->down(Actions::INV_SELECT_LASSO_VERTEX, _lassoPressState))
            && !(hk->down(Actions::ADD_SELECT_LASSO_EDGE, _lassoPressState) ||
                 hk->down(Actions::SUBTRACT_SELECT_LASSO_EDGE, _lassoPressState) ||
                 hk->down(Actions::INV_SELECT_LASSO_EDGE, _lassoPressState)))
            lassoSelecting = false;
    }

    void Selection::updateClickSelection() {
        ZoneScoped
        // click to remove all selection and select one item
        // CRITERIA: when unshifted and clicking on a deselected vertex, mouse click selects it. (clicking on a selected vertex moves it)
        //           *dragging a vertex to create an edge DOES select the vertex
        if (ImGui::IsItemHovered()
            && (press(ADD_SELECT_SINGLE_VERTEX)) ||
            press(SUBTRACT_SELECT_SINGLE_VERTEX) ||
            press(INV_SELECT_SINGLE_VERTEX) ||
            press(ADD_SELECT_SINGLE_EDGE) ||
            press(SUBTRACT_SELECT_SINGLE_EDGE) ||
            press(INV_SELECT_SINGLE_EDGE)) {
            if (!hoveredVertex.empty()) {
                if (vertexSelection.find(hoveredVertex) == vertexSelection.end()) {
                    // not in selection
                    if (press(ADD_SELECT_SINGLE_VERTEX) ||
                        press(INV_SELECT_SINGLE_VERTEX)) // ADD OR INV
                        vertexSelection.insert(hoveredVertex);
                } else {
                    // is in selection
                    if (press(SUBTRACT_SELECT_SINGLE_VERTEX) ||
                        press(INV_SELECT_SINGLE_VERTEX)) // SUBTRACT OR INV
                        vertexSelection.erase(hoveredVertex);
                }
                onChangeSelection = true;
            } else if (!hoveredEdge.empty()) {
                if (edgeSelection.find(hoveredEdge) == edgeSelection.end()) {
                    // not in selection
                    if (press(ADD_SELECT_SINGLE_EDGE) ||
                        press(INV_SELECT_SINGLE_EDGE)) // ADD OR INV
                        edgeSelection.insert(hoveredEdge);
                } else {
                    // is in selection
                    if (press(SUBTRACT_SELECT_SINGLE_EDGE) ||
                        press(INV_SELECT_SINGLE_EDGE)) // SUBTRACT OR INV
                        edgeSelection.erase(hoveredEdge);
                }
                onChangeSelection = true;
            }
        }
    }

    void Selection::updateSelectAll() {
        ZoneScoped
        if (press(ADD_SELECT_ALL_VERTEX))
            for (auto& v : itf->graph.getRead()->props.getVertexPropsList())
                vertexSelection.insert(v.first);
        if (press(ADD_SELECT_ALL_EDGE))
            for (auto& e : itf->graph.getRead()->props.getEdgePropsList())
                edgeSelection.insert(e.first);

        if (press(INV_SELECT_ALL_VERTEX)) {
            for (auto& v : itf->graph.getRead()->props.getVertexPropsList()) {
                if (vertexSelection.find(v.first) == vertexSelection.end())
                    vertexSelection.insert(v.first);
                else
                    vertexSelection.erase(v.first);
            }
        }
        if (press(INV_SELECT_ALL_EDGE)) {
            for (auto& e : itf->graph.getRead()->props.getEdgePropsList()) {
                if (edgeSelection.find(e.first) == edgeSelection.end())
                    edgeSelection.insert(e.first);
                else
                    edgeSelection.erase(e.first);
            }
        }
    }

    float Selection::distance(ImVec2 a, ImVec2 b) {
        float deltaX = b.x - a.x;
        float deltaY = b.y - a.y;
        return sqrt(deltaX * deltaX + deltaY * deltaY);
    }

    float Selection::distanceToALine(ImVec2 p, ImVec2 a, ImVec2 b) {
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

    bool Selection::press(int actionId) { return hk->press(actionId, camera->hoverState); }

    bool Selection::down(int actionId) { return hk->down(actionId, camera->hoverState); }

    bool Selection::release(int actionId) { return hk->release(actionId, camera->hoverState); }

    bool Selection::doubleClick(int actionId) { return hk->doubleClick(actionId, camera->hoverState); }

    bool Selection::hasVelocity(int actionId) { return hk->hasVelocity(actionId, camera->hoverState); }

    float Selection::velocity(int actionId) { return hk->velocity(actionId, camera->hoverState); }

}
#include "GraphView.h"
#include <Editor/Theme/Theme.h>
#include <Core/Objects/Random.h>
#include <Tracy.hpp>

namespace gfn {
    GraphView::GraphView(gfn::Uuid docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs) :
            docId(std::move(docId)),
            itf(itf),
            hk(hk),
            prefs(prefs),
            camera(hk, prefs),
            selection(itf, &camera, hk, prefs),
            renderer(itf, &camera, &selection) {
    }

    void GraphView::update() {
        ZoneScoped

        ImGui::InvisibleButton("graphview_canvas",
                               ImGui::GetContentRegionAvail(),
                               0b11111); // accept all mouse buttons

        ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetWindowPos(),
                                                  ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowWidth(),
                                                         ImGui::GetWindowPos().y + ImGui::GetWindowHeight()),
                                                  IM_COL32(255, 255, 255, 255));

        selection.updateHoverState();
        camera.update();
        selection.updateSelection();

        /// ADD VERTEX PREVIEW
        if (ImGui::IsItemHovered() && selection.down(Actions::ADD_VERTEX_PREVIEW)) {
            ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(),
                                                        camera.map(0.5f + prefs->glow_size), /// TODO
                                                        IM_COL32(255, 0, 255, 100), 0);
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        }

        /// ADD VERTEX
        if (ImGui::IsItemHovered() && selection.press(Actions::ADD_VERTEX)) {
            if (selection.hoveredVertex.empty() && !selection.hoveredEdge.empty()) {
                auto eProp = itf->graph.getRead()->props.getEdgeProps(selection.hoveredEdge);
                execute("rmedge -u=" + eProp->startVertexUuid.value + " -v=" + eProp->endVertexUuid.value);
                auto vId = gfn::createUuid();
                execute("mkvertex -uuid=" + vId);
                execute("setvertexprops -uuid=" + vId +
                        " -key=position -value=(" + std::to_string(selection.mouseCoord.x)
                        + "," + std::to_string(selection.mouseCoord.y));
                execute("mkedge -u=" + vId + " -v=" + eProp->startVertexUuid.value);
                execute("mkedge -u=" + vId + " -v=" + eProp->endVertexUuid.value);
            } else if (selection.hoveredVertex.empty()) {
                auto vId = gfn::createUuid();
                execute("mkvertex -uuid=" + vId);
                execute("setvertexprops -uuid=" + vId +
                        " -key=position -value=\"(" + std::to_string(selection.mouseCoord.x)
                        + ", " + std::to_string(selection.mouseCoord.y) + "\")");
            }
        }

        /// ADD EDGE PREVIEW
        if (ImGui::IsItemHovered() && selection.down(Actions::ADD_EDGE_PREVIEW)) {
            ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(),
                                                        camera.map(0.5f + prefs->glow_size), /// TODO
                                                        IM_COL32(255, 255, 0, 100), 0);
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        }

        /// ADD EDGE
        {
            static bool addingEdge = false;
            static int onAddEdgeState = 0;
            static gfn::Uuid addEdgeVertex;

            if (ImGui::IsItemHovered() && selection.press(Actions::ADD_EDGE)) {
                addingEdge = true;
                onAddEdgeState = camera.hoverState;
                addEdgeVertex.clear();
                if (!selection.hoveredVertex.empty())
                    addEdgeVertex = selection.hoveredVertex;
            }
            if (addingEdge && !addEdgeVertex.empty()) {
                auto uProps = itf->graph.getRead()->props.getVertexProps(addEdgeVertex);
                if (!selection.hoveredVertex.empty()) {
                    auto vProps = itf->graph.getRead()->props.getVertexProps(selection.hoveredVertex);
                    ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value),
                                                        camera.map(vProps->position.value),
                                                        IM_COL32(0, 255, 0, 100), camera.map(0.2f));
                } else {
                    ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value), ImGui::GetMousePos(),
                                                        IM_COL32(0, 255, 0, 100), camera.map(0.2f));
                }
            }
            if (addingEdge && !hk->down(Actions::ADD_EDGE, onAddEdgeState) && !addEdgeVertex.empty()) {
                addingEdge = false;
                if (!selection.hoveredVertex.empty())
                    execute("mkedge -u=" + addEdgeVertex + " -v=" + selection.hoveredVertex);
            }
        }

        /// MOVE VERTEX
        {
            static bool movingVertex = false;
            static int onMoveVertexState = 0;
            static gfn::Uuid moveVertex;

            if (ImGui::IsItemHovered() && selection.press(Actions::MOVE_VERTEX)) {
                if (!selection.hoveredVertex.empty()) {
                    movingVertex = true;
                    onMoveVertexState = camera.hoverState;
                    moveVertex = selection.hoveredVertex;
                    execute("setvertexprops -uuid=" + moveVertex + " -key=pauseUpdate -value=true");
                }
            }
            if (movingVertex) {
                execute("setvertexprops -uuid=" + moveVertex + " -key=position -value=+(" +
                        std::to_string(selection.mouseDelta.x) + "," + std::to_string(selection.mouseDelta.y) + ")");
            }
            if (movingVertex && !hk->down(Actions::MOVE_VERTEX, onMoveVertexState)) {
                movingVertex = false;
                execute("setvertexprops -uuid=" + moveVertex + " -key=pauseUpdate -value=false");
            }
        }

        /// MOVE SELECTION
        {
            static bool movingSelection = false;
            static int onMoveSelectionState = 0;

            if (selection.press(Actions::MOVE_SELECTION)) {
                movingSelection = true;
                onMoveSelectionState = camera.hoverState;
                for (auto& v : selection.vertexSelection)
                    execute("setvertexprops -uuid=" + v + " -key=pauseUpdate -value=true");
            }
            if (movingSelection) {
                for (auto& v : selection.vertexSelection)
                    execute("setvertexprops -uuid=" + v + " -key=position -value=+(" +
                            std::to_string(selection.mouseDelta.x) + "," + std::to_string(selection.mouseDelta.y) + ")");
            }
            if (movingSelection && !hk->down(Actions::MOVE_SELECTION, onMoveSelectionState)) {
                movingSelection = false;
                for (auto& v : selection.vertexSelection)
                    execute("setvertexprops -uuid=" + v + " -key=pauseUpdate -value=false");
            }
        }

        if (ImGui::IsWindowFocused()) {
            /// DELETE HOVERED EDGE
            if (selection.press(Actions::DELETE_HOVERED_EDGE)) {
                if (!selection.hoveredEdge.empty())
                    execute("rmedge -uuid=" + selection.hoveredEdge);
            }

            /// DELETE SELECTED VERTICES
            if (selection.press(Actions::DELETE_EDGES)) {
                if (!selection.edgeSelection.empty()) {
                    for (auto& e : selection.edgeSelection)
                        execute("rmedge -uuid=" + e);
                }
                /// TODO
            }

            /// DELETE HOVERED VERTEX
            if (selection.press(Actions::DELETE_HOVERED_VERTEX)) {
                if (!selection.hoveredVertex.empty())
                    execute("rmvertex -uuid=" + selection.hoveredVertex);
            }

            /// DELETE SELECTED VERTICES
            if (selection.press(Actions::DELETE_VERTICES)) {
                if (!selection.vertexSelection.empty()) {
                    for (auto& v : selection.vertexSelection)
                        execute("rmvertex -uuid=" + v);
                }
            }
        }

        /// COPY
        {
            if (ImGui::IsWindowFocused() && selection.press(Actions::COPY_SELECTION)) {
                nlohmann::json clipJ;
                auto vertexProps = nlohmann::json::array();
                auto edgeProps = nlohmann::json::array();
                auto structure = nlohmann::json::array();
                std::vector<nlohmann::json> vEntry;
                std::unordered_map<gfn::Uuid, int> vertexMapping;
                int vId = 0;

                for (auto& v : selection.vertexSelection) {
                    auto findV = itf->graph.getRead()->props.getVertexProps(v);
                    if (!findV)
                        continue;

                    nlohmann::json vProp;
                    findV->serializeJson(vProp);
                    vertexProps.push_back({std::to_string(vId), vProp});
                    vertexMapping.insert({v, vId});
                    vEntry.push_back(nlohmann::json::array());
                    vId++;
                }
                int eId = 0;
                for (auto& e : selection.edgeSelection) {
                    auto findE = itf->graph.getRead()->props.getEdgeProps(e);
                    if (!findE)
                        continue;
                    auto findU = vertexMapping.find(findE->startVertexUuid.get());
                    auto findV = vertexMapping.find(findE->endVertexUuid.get());
                    if (findU == vertexMapping.end() || findV == vertexMapping.end())
                        continue;
                    nlohmann::json eProp;
                    findE->serializeJson(eProp);
                    edgeProps.push_back({std::to_string(eId), eProp});
                    vEntry[findU->second].push_back({std::to_string(findV->second), std::to_string(eId)});
                    eId++;
                }
                clipJ["vertex props"] = vertexProps;
                clipJ["edge props"] = edgeProps;
                for (int i = 0; i < vEntry.size(); i++)
                    clipJ["structure"].push_back({std::to_string(i), vEntry[i]});

                ImGui::SetClipboardText(clipJ.dump(-1, ' ', true).c_str());
                std::cout << "Copied to clipboard\n";
            }
        }

        /// CUT
        {
            if (ImGui::IsWindowFocused() && selection.press(Actions::CUT_SELECTION)) {
                nlohmann::json clipJ;
                auto vertexProps = nlohmann::json::array();
                auto edgeProps = nlohmann::json::array();
                auto structure = nlohmann::json::array();
                std::vector<nlohmann::json> vEntry;
                std::unordered_map<gfn::Uuid, int> vertexMapping;
                int vId = 0;

                for (auto& v : selection.vertexSelection) {
                    auto findV = itf->graph.getRead()->props.getVertexProps(v);
                    if (!findV)
                        continue;

                    nlohmann::json vProp;
                    findV->serializeJson(vProp);
                    vertexProps.push_back({std::to_string(vId), vProp});
                    vertexMapping.insert({v, vId});
                    vEntry.push_back(nlohmann::json::array());
                    vId++;
                }
                int eId = 0;
                for (auto& e : selection.edgeSelection) {
                    auto findE = itf->graph.getRead()->props.getEdgeProps(e);
                    if (!findE)
                        continue;
                    auto findU = vertexMapping.find(findE->startVertexUuid.get());
                    auto findV = vertexMapping.find(findE->endVertexUuid.get());
                    if (findU == vertexMapping.end() || findV == vertexMapping.end())
                        continue;
                    nlohmann::json eProp;
                    findE->serializeJson(eProp);
                    edgeProps.push_back({std::to_string(eId), eProp});
                    vEntry[findU->second].push_back({std::to_string(findV->second), std::to_string(eId)});
                    eId++;
                }
                clipJ["vertex props"] = vertexProps;
                clipJ["edge props"] = edgeProps;
                for (int i = 0; i < vEntry.size(); i++)
                    clipJ["structure"].push_back({std::to_string(i), vEntry[i]});

                ImGui::SetClipboardText(clipJ.dump(-1, ' ', true).c_str());
                std::cout << "Copied to clipboard\n";

                if (!selection.edgeSelection.empty()) {
                    for (auto& e : selection.edgeSelection)
                        execute("rmedge -uuid=" + e);
                }
                if (!selection.vertexSelection.empty()) {
                    for (auto& v : selection.vertexSelection)
                        execute("rmvertex -uuid=" + v);
                }
            }
        }

        /// PASTE
        {
            if (ImGui::IsWindowFocused() && selection.press(Actions::PASTE_SELECTION)) {
                /// paste will be done in core, send the json to core through commands
                /// TODO
            }
        }



        /*if (!selection.mouseClickVertex[ImGuiMouseButton_Left].empty() &&
            selection.vertexSelection.empty() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            auto uProps = itf->graph.getRead()->props.getVertexProps(
                    selection.mouseClickVertex[ImGuiMouseButton_Left]);

        }*/

        if (!selection.vertexSelection.empty()) {
            for (auto& v: selection.vertexSelection) {
                auto props = itf->graph.getRead()->props.getVertexProps(v);
                if (props && props->enabled.get()) {
                    ImGui::GetWindowDrawList()->AddCircleFilled(
                            camera.map(props->position.value), camera.map(props->radius.value + prefs->glow_size),
                            IM_COL32(0, 135, 255, 255),
                            0);
                }
            }
        }

        if (!selection.edgeSelection.empty()) {
            for (auto& e : selection.edgeSelection) {
                auto edgeProps = itf->graph.getRead()->props.getEdgeProps(e);
                if (edgeProps) {
                    auto uProps = itf->graph.getRead()->props.getVertexProps(edgeProps->startVertexUuid.value);
                    auto vProps = itf->graph.getRead()->props.getVertexProps(edgeProps->endVertexUuid.value);
                    if (uProps && vProps && edgeProps->enabled.get())
                        ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value), camera.map(vProps->position.value),
                                                            IM_COL32(0, 135, 255, 255),
                                                            camera.map(edgeProps->thickness.value + prefs->glow_size * 2.0));
                }
            }
        }

        if (!selection.hoveredVertex.empty()) {
            auto props = itf->graph.getRead()->props.getVertexProps(selection.hoveredVertex);
            ImGui::GetWindowDrawList()->AddCircleFilled(camera.map(props->position.value), camera.map(props->radius.value + prefs->glow_size),
                                                        IM_COL32(0, 255, 255, 100), 0);
        } else if (!selection.hoveredEdge.empty()) {
            auto edgeProps = itf->graph.getRead()->props.getEdgeProps(selection.hoveredEdge);
            auto uProps = itf->graph.getRead()->props.getVertexProps(edgeProps->startVertexUuid.value);
            auto vProps = itf->graph.getRead()->props.getVertexProps(edgeProps->endVertexUuid.value);
            ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value), camera.map(vProps->position.value),
                                                IM_COL32(0, 255, 255, 100),
                                                camera.map(edgeProps->thickness.value + prefs->glow_size * 2.0));
        }

        renderer.drawEdges();
        renderer.drawVertices();

    }

    void GraphView::execute(const std::string& cmd) { commands.push(cmd); }

}
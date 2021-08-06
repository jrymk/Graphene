#pragma once

#include <vector>
#include <imgui.h>
#include <Core/Interface/Interface.h>
#include <Editor/Preferences/Preferences.h>
#include <Editor/Document/GraphView/Camera.h>
#include <Editor/Document/GraphView/Renderer.hpp>
#include <Editor/Document/GraphView/Selection.hpp>
#include <earcut.hpp>
#include <minitrace.h>

namespace gfn {
///@brief renders and handles interaction to a specified interface
    class GraphView {
    public:
        gfn::Uuid docId;
        gfn::Interface* itf;
        gfn::HKHandler* hk;
        gfn::Preferences* prefs;

        Camera camera;
        Selection selection;
        Renderer renderer;

        GraphView(const gfn::Uuid& docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs) :
                docId(docId),
                itf(itf),
                hk(hk),
                prefs(prefs),
                camera(hk, prefs),
                selection(itf, &camera, hk, prefs),
                renderer(itf, &camera) {
        }

        void update() {
            MTR_SCOPE("graphview", "graphview");
            assert(itf && preferences); // did you forget to bind interface or preferences?

            // ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            // auto binding = preferences->getKeyBind().getBinding(gfn::bindings::ACTION_CAMERA_PAN);
            ImGui::InvisibleButton("graphview_canvas",
                                   ImVec2(std::max(ImGui::GetContentRegionAvail().x, 10.0f),
                                          std::max(ImGui::GetContentRegionAvail().y, 10.0f)),
                                   0b11111); // accept all mouse buttons

            //if (ImGui::IsWindowFocused()) {
            camera.update(selection.mouseHoverState);
            selection.update();

            if (!selection.mouseClickVertex[ImGuiMouseButton_Left].empty() &&
                selection.vertexSelection.empty() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                auto uProps = itf->graph.getRead()->props.getVertexProps(
                        selection.mouseClickVertex[ImGuiMouseButton_Left]);
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

            if (!selection.vertexSelection.empty()) {
                for (auto& v : selection.vertexSelection) {
                    auto props = itf->graph.getRead()->props.getVertexProps(v);
                    if (props) {
                        ImGui::GetWindowDrawList()->AddCircleFilled(camera.map(props->position.value),
                                                                    camera.map(
                                                                            props->radius.value +
                                                                            prefs->glow_size),
                                                                    props->enabled.get() ? IM_COL32(0, 135, 255, 255)
                                                                                         : IM_COL32(100, 100, 100, 120),
                                                                    0);
                    }
                }
            }
            if (!selection.edgeSelection.empty()) {
                for (auto& e : selection.edgeSelection) {
                    auto props = itf->graph.getRead()->props.getEdgeProps(e);
                    if (props) {
                        auto edgeProps = itf->graph.getRead()->props.getEdgeProps(e);
                        auto uProps = itf->graph.getRead()->props.getVertexProps(edgeProps->startVertexUuid.value);
                        auto vProps = itf->graph.getRead()->props.getVertexProps(edgeProps->endVertexUuid.value);
                        ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value),
                                                            camera.map(vProps->position.value),
                                                            props->enabled.get() ? IM_COL32(0, 135, 255, 255)
                                                                                 : IM_COL32(100, 100, 100, 120),
                                                            camera.map(
                                                                    edgeProps->thickness.value +
                                                                    prefs->glow_size * 2.0));
                    }
                }
            }

            if (!selection.hoveredVertex.empty()) {
                auto props = itf->graph.getRead()->props.getVertexProps(selection.hoveredVertex);
                ImGui::GetWindowDrawList()->AddCircleFilled(camera.map(props->position.value),
                                                            camera.map(props->radius.value + prefs->glow_size),
                                                            IM_COL32(0, 255, 255, 100), 0);
            } else if (!selection.hoveredEdge.empty()) {
                auto edgeProps = itf->graph.getRead()->props.getEdgeProps(selection.hoveredEdge);
                auto uProps = itf->graph.getRead()->props.getVertexProps(edgeProps->startVertexUuid.value);
                auto vProps = itf->graph.getRead()->props.getVertexProps(edgeProps->endVertexUuid.value);
                ImGui::GetWindowDrawList()->AddLine(camera.map(uProps->position.value),
                                                    camera.map(vProps->position.value),
                                                    IM_COL32(0, 255, 255, 100),
                                                    camera.map(
                                                            edgeProps->thickness.value + prefs->glow_size * 2.0));
            }

            {
                MTR_SCOPE("graphview", "render objects");
                renderer.drawEdges();
                renderer.drawVertices();
            }
            //            if (selection.lassoSelecting) {
            //                for (auto& e : interface->properties.getRead()->getEdgePropsList()) {
            //                    ImGui::GetWindowDrawList()->AddCircleFilled(camera.map(e.second.position.get()),
            //                                                                camera.map(0.2f),
            //                                                                IM_COL32(255, 150, 150, 255), 0);
            //                }
            //            }
            //}
            // ImGui::PopStyleVar(1);
        }
    };
} // namespace gfn::graphview
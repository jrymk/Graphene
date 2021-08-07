#include "GraphView.h"
#include <Editor/Theme/Theme.h>
#include <Tracy.hpp>

namespace gfn {
    GraphView::GraphView(gfn::Uuid docId, gfn::Interface* itf, gfn::HKHandler* hk, gfn::Preferences* prefs) :
            docId(std::move(docId)),
            itf(itf),
            hk(hk),
            prefs(prefs),
            camera(hk, prefs),
            selection(itf, &camera, hk, prefs),
            renderer(itf, &camera) {
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

        /// ADD EDGE PREVIEW
        if (ImGui::IsItemHovered() && selection.down(Actions::ADD_EDGE_PREVIEW)) {
            ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(),
                                                        camera.map(0.5f + prefs->glow_size), /// TODO
                                                        IM_COL32(255, 0, 255, 100), 0);
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
        }


        /*if (!selection.mouseClickVertex[ImGuiMouseButton_Left].empty() &&
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
        }*/

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

        renderer.drawEdges();
        renderer.drawVertices();
    }
}
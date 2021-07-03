#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "View.hpp"
#include "Overlay.hpp"
#include "Renderer.hpp"
#include "ContextMenu.hpp"
#include "Controls.hpp"
#include "../RenderUtils/ConvexHull.hpp"

namespace gph {
    namespace GraphView {

        void updateKeyboardShortcuts() {
            // keyboard shortcut to toggle auto adjust view
            if (ImGui::IsKeyPressed('A', false))
                Controls::enableAutoAdjustView = !Controls::enableAutoAdjustView;
            if (ImGui::IsKeyPressed('V', true))
                Graphene::core->getGraphObj()->newVertex();
        }

        void show(::gph::Core* core) {
            Graphene::core = core;

            ImGui::SetNextWindowSizeConstraints(ImVec2(300, 350), ImVec2(FLT_MAX, FLT_MAX));

            ImGui::Begin(u8"Graph View", 0, ImGuiWindowFlags_NoCollapse);

            ImGui::Checkbox("Enable render", &Controls::enableRender);
            ImGui::SameLine();
            ImGui::Checkbox("Show grid", &Controls::gridVisible);
            ImGui::SameLine();
            ImGui::Checkbox("Auto adjust view (A)", &Controls::enableAutoAdjustView);

            if (!Controls::enableAutoAdjustView) {
                ImGui::SameLine();
                if (ImGui::Button("Reset view")) {
                    View::zoomTarget = 1.0f;
                    View::centerContext = ImVec2(0.5f, 0.5f);
                }
            }

            View::canvasBegin();

            //Graphene::core->getGraphObj()->updateConnectedComponent(); // continuously running this command is not a good idea

            if (Controls::enableAutoAdjustView)
                View::autoAdjustView();
            if (!Controls::contextMenuOpen)
                updateKeyboardShortcuts();

            if (!Controls::contextMenuOpen) {
                Controls::updateHoveredVertex();
                if (Controls::leftMouseSelectionVertex != nullptr) {
                    Controls::leftMouseSelectionVertex->directMove(
                            Vec2f(
                                    ImGui::GetIO().MouseDelta.x / View::canvasFrameSize / View::zoomLevel,
                                    -ImGui::GetIO().MouseDelta.y / View::canvasFrameSize / View::zoomLevel
                            )
                    );
                    Controls::leftMouseSelectionVertex->flushMove(0.0f);
                }
                Controls::updateHoveredComponent();
            }
            Controls::updateMouse();

            if (Controls::enableRender) {
                View::updateCamera();
                View::updateMouseCursor();

                if (Controls::gridVisible)
                    Renderer::drawGrid();

                Renderer::drawComponents();
                Renderer::drawEdges();
                Renderer::drawVertices();
            } else {
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 100, 100, 255));
                std::string tempStr("Rendering is disabled");
                char* label = new char[tempStr.length() + 1];
                strcpy(label, tempStr.c_str());
                ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
                ImVec2 labelAlign(0.5f, 0.5f);
                const ImRect bb(View::canvasCursor, {View::canvasCursor.x + View::canvasSize.x, View::canvasCursor.y + View::canvasSize.y});

                ImGui::RenderTextClipped(bb.Min, bb.Max, label, 0, &labelSize, labelAlign, &bb);
                ImGui::PopStyleColor(1);
            }

            ContextMenu::update();
            Overlay::draw();

            View::canvasEnd();
        }
    }
}
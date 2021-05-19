#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "Include.h"
#include "../utils/ExceptionHandler.hpp"
#include "../graphene/Include.h"
#include "Themes.hpp"

namespace Gui {

    namespace DebugWindow {

        void show(Graphene::Graph* graph) {

            ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin("Debug");

            graph->debugVertexHighlight = nullptr;

            ImGui::Text("Adjacency List");
            for (auto &uIt : graph->graph) {
                ImGui::SetNextItemOpen(true);
                if (ImGui::TreeNode((uIt.first->UUID).c_str())) {
                    if (ImGui::IsItemHovered())
                        graph->debugVertexHighlight = uIt.first;
                    for (auto &vIt : uIt.second) {
                        ImGui::Text(vIt.first->UUID.c_str());
                        if (ImGui::IsItemHovered())
                            graph->debugVertexHighlight = vIt.first;
                    }
                    ImGui::TreePop();
                }
            }

            ImGui::Text("Components");
            for (auto &cIt : graph->components) {
                ImGui::SetNextItemOpen(true);
                ImGui::PushStyleColor(ImGuiCol_Text, cIt->color);
                if (ImGui::TreeNode(("Component: " + cIt->UUID).c_str())) {
                    for (auto &uIt : cIt->adjList) {
                        ImGui::SetNextItemOpen(true);
                        if (ImGui::TreeNode(("Vertex: " + uIt.first->UUID).c_str())) {
                            if (ImGui::IsItemHovered())
                                graph->debugVertexHighlight = uIt.first;
                            for (auto &vIt : uIt.second) {
                                ImGui::Text(vIt->UUID.c_str());
                                if (ImGui::IsItemHovered())
                                    graph->debugVertexHighlight = vIt;
                            }
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::PopStyleColor(1);
            }

            ImGui::End();

        }

    }

}
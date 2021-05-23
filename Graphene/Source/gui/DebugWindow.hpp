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

            if (ImGui::Button("Force update components"))
                graph->updateConnectedComponent();

            ImGui::Text(("Components (" + std::to_string(graph->graph.size()) + ")").c_str());
            if (graph->graph.empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.25f, 0.25f, 1.0f));
                ImGui::Text("Nothing to show here");
                ImGui::PopStyleColor(1);
            } else {
                for (auto &cIt : graph->components) {
                    ImGui::SetNextItemOpen(true);
                    ImGui::PushStyleColor(ImGuiCol_Text, cIt->color);
                    if (ImGui::TreeNode(("Component: " + cIt->getUUID()).c_str())) {
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
            }

            ImGui::Text("Adjacency List");
            if (graph->graph.empty()) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.25f, 0.25f, 1.0f));
                ImGui::Text("Nothing to show here");
                ImGui::PopStyleColor(1);
            } else {
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
            }

            ImGui::End();

        }

    }

}
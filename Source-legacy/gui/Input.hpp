#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../utils/ExceptionHandler.hpp"
#include "../graphene/Include.h"
#include "Window.hpp"
#include "RawInput.hpp"
#include "Themes.hpp"

namespace Gui {

	namespace Input {

		void show(Graphene::Core* core) {

			ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin("Input");
			
			showRawInput(core);

			/*if (ImGui::BeginTabItem("Adjacency Matrix")) {
				bool check_update = false;
				int prev_vertices = graphene_vertices;

				ImGui::SliderInt("Vertices", &graphene_vertices, 0, 20, "%d", 0);
				if (prev_vertices != graphene_vertices)
					check_update = true;

				graphene_core->getGraph()->resize(graphene_vertices);
				for (int i = 0; i < graphene_vertices; i++)
					graphene_graph[i].resize(graphene_vertices);


				if (ImGui::BeginTable("checkbocGridTable", graphene_vertices + 1, 1 << 13, ImVec2(0, 0), 0.0)) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					for (int column = 0; column < graphene_vertices; column++) {
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(column).c_str());
					}
					for (int row = 0; row < graphene_vertices; row++) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(row).c_str());

						for (int column = 0; column < graphene_vertices; column++) {
							ImGui::TableNextColumn();

							bool check = graphene_graph[row][column];
							bool prevCheck = check;
							ImGui::Checkbox((std::string("##") + std::to_string(row) + std::string(",") + std::to_string(column)).c_str(), &check);
							graphene_graph[row][column] = check;
							graphene_graph[column][row] = check;

							if (prevCheck != check)
								check_update = true;
						}

					}
					ImGui::EndTable();
				}

				ImGui::EndTabItem();


				if (check_update) {
					int tickCount = 0;
					std::vector<std::pair<std::pair<int, int>, bool>> temp_graph;

					for (int i = 0; i < graphene_vertices; i++) {
						for (int j = 0; j < graphene_vertices; j++) {
							if (graphene_graph[i][j]) {
								tickCount++;
								temp_core->getGraph()->push_back({ {i, j}, false });
							}
						}
					}

					core->getGraph()->importEdges(graphene_vertices, temp_graph);
				}

			}*/

			ImGui::End();

		}

	}

}
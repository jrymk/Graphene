#pragma once

#include "Window.hpp"
#include "Toolbar.hpp"
#include "Input.hpp"
#include "RawInput.hpp"
#include "AdjMatrixInput.hpp"
#include "Themes.hpp"
#include "DebugWindow.hpp"
#include "LogWindow.hpp"
#include "GraphView/GraphView.hpp"

namespace Gui {

	Graphene::Core* core;

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



}



/// Graveyard

		/*{
		ImGui::Begin("Vertex Info");

		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;

		ImVec2 outer_size = ImVec2(0.0f, 0.0f);
		if (ImGui::BeginTable("table_scrollx", 3, flags, outer_size)) {

			ImGui::TableSetupScrollFreeze(1, 1);
			ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
			ImGui::TableSetupColumn("PosX");
			ImGui::TableSetupColumn("PosY");

			ImGui::TableHeadersRow();
			for (int vertex = 0; vertex < graph.vertexCount; vertex++) {
				ImGui::TableNextRow();

				for (int column = 0; column < 3; column++) {

					if (!ImGui::TableSetColumnIndex(column) && column > 0)
						continue;

					switch (column) {
					case 0:
						ImGui::Text("%d", vertex);
						break;
					case 1:
						ImGui::Text("%f", graph.vertices[vertex]->getCoord().x);
						break;
					case 2:
						ImGui::Text("%f", graph.vertices[vertex]->getCoord().y);
						break;

					}

				}

			}
			ImGui::EndTable();
		}

		ImGui::End();
		}*/

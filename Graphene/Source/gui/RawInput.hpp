#pragma once

namespace Gui {

	namespace Input {

		void showRawInput(Graphene::Core* core) {

			static char text[65536] = "";
			static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

			static int vertexCount = 0;

			ImGui::SliderInt("Vertices (C/V)", &vertexCount, 0, 50, "%d", 0);

			if (ImGui::IsKeyPressed('C', true) && vertexCount > 0)
				vertexCount--;
			if (ImGui::IsKeyPressed('V', true))
				vertexCount++;

			bool pendingUpdate = false;
			if (ImGui::Button("Wipe and reset (R)") || ImGui::IsKeyPressed('R', false)) {
				core->boundGraph()->resetVertices();
                pendingUpdate = true;
			}

			core->boundGraph()->resizeVertices(vertexCount);

			if (ImGui::Button("Update") || pendingUpdate) {
				std::stringstream ss;
				ss << text;
				core->boundGraph()->clearAllEdges();
				int u, v;
				while (ss >> u >> v) {
					if (u < core->boundGraph()->getVertexCount() && v < core->boundGraph()->getVertexCount())
						core->boundGraph()->addEdge(u, v);
				}
			}

			// add or delete edge operation happened
			if (core->pendingInputUpdate) {
				std::stringstream ss;

				Graphene::EdgeIter it(core->boundGraph());
				while (it.next())
					ss << it.u->getNumber() << " " << it.v->getNumber() << "\n";

				const std::string tempStr = ss.str();
				strcpy(text, tempStr.c_str());

				core->pendingInputUpdate = false;
			}

			ImGui::InputTextMultiline("##source", text, 65536, ImVec2(-FLT_MIN, -FLT_MIN), flags);

		}


	}

}


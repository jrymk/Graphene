#pragma once

namespace Gui {

	namespace Input {

		void showRawInput(Graphene::Core* core) {

			static char text[65536] = "";
			static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

			static unsigned int prevEdgeSize = core->getGraph()->edges.size();

			static int v, e = 0, a, b;

			ImGui::SliderInt("Vertices", &v, 0, 50, "%d", 0);

			bool pendingUpdate = false;

			if (ImGui::Button("Wipe and reset")) {
				for (int i = 0; i < core->getGraph()->vertices.size(); i++) {
					delete core->getGraph()->vertices[i];
				}
				core->getGraph()->vertices.clear();
				for (int i = 0; i < v; i++) {
					Graphene::Vertex* vertex = new Graphene::Vertex(i);
					core->getGraph()->vertices.emplace_back(vertex);
				}

				pendingUpdate = true;
			}

			if (ImGui::Button("Update") || pendingUpdate) {
				std::stringstream ss;
				ss << text;

				core->getGraph()->edges.clear();
				for (int i = 0; i < core->getGraph()->adjList.size(); i++)
					core->getGraph()->adjList[i].clear();
				core->getGraph()->edgeCount = 0;

				while (ss >> a >> b) {
					e++;
					core->getGraph()->edgeCount++;
					core->getGraph()->edges.emplace_back(core->getGraph()->vertices[a], core->getGraph()->vertices[b], false);
					core->getGraph()->adjList[a].push_back(core->getGraph()->vertices[b]);
					core->getGraph()->adjList[b].push_back(core->getGraph()->vertices[a]);
				}

			}
			

			if (v > core->getGraph()->vertexCount) {
				for (int i = core->getGraph()->vertexCount; i < v; i++) {
					Graphene::Vertex* vertex = new Graphene::Vertex(i);
					core->getGraph()->vertices.emplace_back(vertex);
				}
			}
			else {
				for (int i = core->getGraph()->vertexCount - 1; i >= v; i--) {
					delete core->getGraph()->vertices[i];
				}
			}

			core->getGraph()->vertexCount = v;
			core->getGraph()->vertices.resize(v);
			core->getGraph()->adjList.resize(v);


			if (core->pendingInputUpdate) {
				std::stringstream ss;
				ss << core->getGraph()->vertices.size() << " " << core->getGraph()->edges.size() << "\n";
				for (int i = 0; i < core->getGraph()->edges.size(); i++) {
					ss << core->getGraph()->edges[i].startingVertex->getNumber() << " " <<
						core->getGraph()->edges[i].endingVertex->getNumber() << "\n";
				}

				const std::string tempStr = ss.str();
				std::strcpy(text, tempStr.c_str());

				core->pendingInputUpdate = false;
			}

			prevEdgeSize = core->getGraph()->edges.size();

			std::cerr << text << "\n";

			ImGui::InputTextMultiline("##source", text, 65536, ImVec2(-FLT_MIN, -FLT_MIN), flags);

		}

	}

}
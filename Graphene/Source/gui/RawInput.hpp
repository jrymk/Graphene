#pragma once

namespace Gui {

    namespace Input {

        int vertexCount = 0;

        void showRawInput(Graphene::Core* core) {

            static char text[65536] = "";
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

            //ImGui::SliderInt("Vertices (C/V)", &vertexCount, 0, 50, "%d", 0);

            //if (ImGui::IsKeyPressed('C', true) && vertexCount > 0)
                //core->graph->deleteVertex(core->graph.)
            /*if (ImGui::IsKeyPressed('V', true))
                core->graph->newVertex();*/

            bool pendingUpdate = false;
            /*if (ImGui::Button("Wipe and reset (R)") || ImGui::IsKeyPressed('R', false)) {
                core->graph->resetVertices();
                pendingUpdate = true;
            }*/

            /*if (core->graph->getVertexCount() != vertexCount) {
                core->graph->resizeVertices(vertexCount);
                pendingUpdate = true;
            }*/

            /*if (ImGui::Button("Update") || pendingUpdate) {
                std::stringstream ss;
                ss << text;
                core->graph->clearAllEdges();
                int u, v;
                while (ss >> u >> v) {
                    if (u < core->graph->getVertexCount() && v < core->graph->getVertexCount())
                        core->graph->newEdge(u, v);
                }
                core->graph->evalConnectedComponent();

            }*/

            // add or delete edge operation happened
            if (core->pendingInputUpdate) {
                std::stringstream ss;

                Graphene::EdgeIter it(core->graph);
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


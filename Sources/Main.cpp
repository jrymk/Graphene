#include <string>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <Editor/Logs.hpp>
#include <Document/Document.hpp>
#include <Editor/Editor.hpp>

int main(int argc, char* argv[]) {
    gfn::window::launchWindow();
    gfn::editor::init();

    while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
        gfn::window::preFrame();

        static bool first = true;
        if (first) {
            gfn::editor::loadDragAndDrop(argc, argv);
            first = false;
        }

        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();

        auto fDoc = gfn::editor::getActiveDocument();

        ImGui::Begin("Hot key debugger");
        for (int i = 0; i < gfn::keybind::actions.size(); i++) {
            ImGui::Text(gfn::keybind::actions[i].c_str());
            if (gfn::editor::hotKeyPress[i]) {
                ImGui::SameLine();
                ImGui::Text("PRESS");
            }
            if (gfn::editor::hotKeyHold[i]) {
                ImGui::SameLine();
                ImGui::Text("HOLD");
            }
            if (gfn::editor::hotKeyRelease[i]) {
                ImGui::SameLine();
                ImGui::Text("RELEASE");
            }
        }
        ImGui::End();

        ImGui::Begin("Command centre");

        if (ImGui::Button("New file"))
            gfn::editor::newFile();
        ImGui::SameLine();
        if (ImGui::Button("Open file")) {
            ImGui::OpenPopup("Open File");
            gfn::editor::openFile();
        }
        if (!gfn::editor::activeDocumentUuid.empty()) {
            ImGui::SameLine();
            if (gfn::editor::getActiveDocument()->filePath.empty()) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 255));
            }
            if (ImGui::Button("Save File"))
                gfn::editor::saveFile();
            if (gfn::editor::getActiveDocument()->filePath.empty()) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor(1);
            }
            ImGui::SameLine();
            if (ImGui::Button("Save As File")) {
                gfn::editor::saveAsFile(gfn::editor::activeDocumentUuid);
                ImGui::OpenPopup("Save As File");
            }
        }

        ImGui::Separator();

        static bool showKeyBindWindow = false;
        if (ImGui::Button("Key binds")) {
            gfn::editor::keybind.showWindow = true;
            ImGui::OpenPopup("Key Binds");
        }
        if (gfn::editor::keybind.showWindow)
            gfn::editor::keybind.showKeybindSetup();

        ImGui::Separator();

        //if (gfn::editor::isOpeningFile)

        //if (gfn::editor::isSavingAsFile)

        static gfn::Uuid prevActiveDocId;
        if (fDoc) {
            static float c1p;
            static float c2p;
            static float c3p;
            static float c4p;
            static float c5p;
            static float c6p;
            //if (prevActiveDocId != fDoc->docId) {
            prevActiveDocId = fDoc->docId;
            c1p = float(fDoc->interface.configs.getRead()->c1.value);
            c2p = float(fDoc->interface.configs.getRead()->c2.value);
            c3p = float(fDoc->interface.configs.getRead()->c3.value);
            c4p = float(fDoc->interface.configs.getRead()->c4.value);
            c5p = float(fDoc->interface.configs.getRead()->c5.value);
            c6p = float(fDoc->interface.configs.getRead()->c6.value);
            //}
            fDoc->interface.configs.readDone();
            float c1 = c1p;
            float c2 = c2p;
            float c3 = c3p;
            float c4 = c4p;
            float c5 = c5p;
            float c6 = c6p;
            ImGui::SliderFloat("c1", &c1, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("c2", &c2, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("c3", &c3, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("c4", &c4, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("c5", &c5, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            ImGui::SliderFloat("c6", &c6, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
            if (c1p != c1)
                gfn::editor::execute("configs -c1=" + std::to_string(c1));
            if (c2p != c2)
                gfn::editor::execute("configs -c2=" + std::to_string(c2));
            if (c3p != c3)
                gfn::editor::execute("configs -c3=" + std::to_string(c3));
            if (c4p != c4)
                gfn::editor::execute("configs -c4=" + std::to_string(c4));
            if (c5p != c5)
                gfn::editor::execute("configs -c5=" + std::to_string(c5));
            if (c6p != c6)
                gfn::editor::execute("configs -c6=" + std::to_string(c6));
            c1p = c1;
            c2p = c2;
            c3p = c3;
            c4p = c4;
            c5p = c5;
            c6p = c6;
        }

        if (fDoc) {
            if (ImGui::TreeNode("UserGraph adjacency list")) {
                if (ImGui::TreeNode("Vertices")) {
                    auto g = fDoc->interface.usergraph.getRead();
                    for (auto& u : g->getAdjList()) {
                        if (ImGui::TreeNode(u.first.c_str())) {
                            for (auto& v : u.second) {
                                ImGui::TreeNode(v.first.c_str());
                                for (auto& e : v.second) {
                                    ImGui::Text(e.c_str());
                                }
                                ImGui::TreePop();
                            }
                            ImGui::TreePop();
                        }
                    }

                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Properties")) {
                if (ImGui::TreeNode("Vertices")) {
                    for (auto& v : fDoc->interface.properties.getRead()->getVertexPropList()) {
                        if (ImGui::TreeNode(v.first.c_str())) {
                            ImGui::Text(("enabled: " + std::string(v.second.enabled.value ? "true" : "false")).c_str());
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Edges")) {
                    for (auto& v : fDoc->interface.properties.getRead()->getEdgePropList()) {
                        if (ImGui::TreeNode(v.first.c_str())) {
                            ImGui::Text(("enabled: " + std::string(v.second.enabled.value ? "true" : "false")).c_str());
                            ImGui::Text(("start: " + v.second.startVertexUuid.value).c_str());
                            ImGui::Text(("end: " + v.second.endVertexUuid.value).c_str());
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
        }
        // if (ImGui::Button("New vertex"))
        // core.parser.execute("graph new vertex"); // No, you can not run this here
        gfn::editor::updateMainWindow();
        ImGui::End();

        gfn::editor::showPropertiesPanel(); // must go before updateDocuments

        gfn::editor::updateDocuments();

        gfn::window::postFrame();
    }

    gfn::editor::terminate();

    gfn::window::closeWindow();
    return 0;
}
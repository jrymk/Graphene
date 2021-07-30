#include <stdio.h>
#include <string>
#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <Logging/Logging.hpp>
#include <Editor/Logs.hpp>
#include <Objects/Uuid.hpp>
#include <Interface/Interface.hpp>
#include <System/Random/Random.hpp>
// #include <Editor/Logs.hpp>
#include <Document/Document.hpp>
#include <Preferences/Preferences.hpp>
#include <Editor/EditorManager.hpp>

int main() {
    gfn::window::launchWindow();

    gfn::editor::startup();

    /*{
        gfn::editor::newFile("giraffe(fixed)");
        std::string testInput = {""
                                 "7 10 "
                                 "0 1 "
                                 "0 2 "
                                 "0 4 "
                                 "1 2 "
                                 "1 4 "
                                 "2 3 "
                                 "2 5 "
                                 "3 6 "
                                 "4 5 "
                                 "5 6 "};
        gfn::editor::execute("cd -d giraffe(fixed)");
        std::stringstream ss(testInput);
        int v, e;
        ss >> v >> e;
        for (int i = 0; i < v; i++)
            gfn::editor::execute("mkvertex -name " + std::to_string(i));
        for (int i = 0; i < e; i++) {
            int s, t;
            ss >> s >> t;
            gfn::editor::execute("mkedge -uname " + std::to_string(s) + " -vname " + std::to_string(t));
        }
    }*/
    {
        gfn::editor::newFile("kangaroo(small tree)");
        int c = 64;
        gfn::editor::execute("cd -d kangaroo(small tree)");
        gfn::editor::execute("mkvertex -name " + std::to_string(0));
        for (int j = 0; j < c; j++) {
            gfn::editor::execute("mkvertex -name " + std::to_string(j + 1));
            std::uniform_int_distribution<int> dis(0, j);
            gfn::editor::execute("mkedge -uname " + std::to_string(j + 1) + " -vname " +
                                 std::to_string(dis(gfn::system::random::getEngine())));
        }
    }
    {
        gfn::editor::newFile("elephant(grid)");
        int c = 32;
        gfn::editor::execute("cd -d elephant(grid)");
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < c; j++)
                gfn::editor::execute("mkvertex -name " + std::to_string(i) + "*" + std::to_string(j));
        }
        for (int i = 0; i < c; i++) {
            // PLANE
            for (int j = 0; j < c - 1; j++) {
                gfn::editor::execute("mkedge -uname " + std::to_string(i) + "*" + std::to_string(j) + " -vname " +
                                     std::to_string(i) + "*" + std::to_string(j + 1));
                gfn::editor::execute("mkedge -uname " + std::to_string(j) + "*" + std::to_string(i) + " -vname " +
                                     std::to_string(j + 1) + "*" + std::to_string(i));
            }
            // DONUT
            /*gfn::editor::execute("mkedge -uname " + std::to_string(i) + "*" + std::to_string(c - 1) + " -vname " +
                                 std::to_string(i) + "*" + std::to_string(0));
            gfn::editor::execute("mkedge -uname " + std::to_string(0) + "*" + std::to_string(i) + " -vname " +
                                 std::to_string(c - 1) + "*" + std::to_string(i));*/
        }
    }
    {
        gfn::editor::newFile("koala(big ass tree)");
        int c = 1024;
        gfn::editor::execute("cd -d koala(big ass tree)");
        gfn::editor::execute("mkvertex -name " + std::to_string(0));
        for (int j = 0; j < c; j++) {
            gfn::editor::execute("mkvertex -name " + std::to_string(j + 1));
            std::uniform_int_distribution<int> dis(0, j);
            gfn::editor::execute("mkedge -uname " + std::to_string(j + 1) + " -vname " +
                                 std::to_string(dis(gfn::system::random::getEngine())));
        }
    }
    {
        gfn::editor::newFile("graphene");
        int c = 16;
        gfn::editor::execute("cd -d graphene");
        for (int i = 0; i < c; i++) {
            for (int j = 0; j < c; j++)
                gfn::editor::execute("mkvertex -name " + std::to_string(i) + "*" + std::to_string(j));
        }
        for (int i = 0; i < c; i++) {
            // PLANE
            for (int j = 0; j < c - 1; j++) {
                if ((i + j) % 2) {
                    gfn::editor::execute("mkedge -uname " + std::to_string(i) + "*" + std::to_string(j) + " -vname " +
                                         std::to_string(i) + "*" + std::to_string(j + 1));
                }
                gfn::editor::execute("mkedge -uname " + std::to_string(j) + "*" + std::to_string(i) + " -vname " +
                                     std::to_string(j + 1) + "*" + std::to_string(i));
            }
            // DONUT
            /*gfn::editor::execute("mkedge -uname " + std::to_string(i) + "*" + std::to_string(c - 1) + " -vname " +
                                 std::to_string(i) + "*" + std::to_string(0));
            gfn::editor::execute("mkedge -uname " + std::to_string(0) + "*" + std::to_string(i) + " -vname " +
                                 std::to_string(c - 1) + "*" + std::to_string(i));*/
        }
    }

    gfn::editor::newFile("Untitled(1)");

    while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
        gfn::window::preFrame();

        //ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();

        gfn::editor::update();

        // gfn::editor::logs::show();

        // auto stat = interface.properties.getOutpaceStatistics();
        // static int v;

        // if (!interface.properties.isReadBufferRead())
        //    v = stat.second - stat.first;
        // preferences.getKeyBind().showEnrollPopup(gfn::keybind::ACTION_CAMERA_PAN);

        ImGui::Begin("Command centre");

        if (ImGui::Button("New file"))
            gfn::editor::newFile(gfn::uuid::createUuid());

        if (gfn::editor::focusedDocument)
            ImGui::Text(("Focused document: " + gfn::editor::focusedDocument->documentUuid).c_str());

        /*ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(255, 255, 255, 255));
        if (v > 0)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(200, 255, 200, 255));
        else if (v == 0)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 255, 200, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 200, 200, 255));

        ImGui::SliderInt("Thread outpace", &v, -10000000, 10000000, "%d",
                         ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

        ImGui::PopStyleColor(2);*/

        if (gfn::editor::focusedDocument) {
            static float c1p;
            static float c2p;
            static float c3p;
            static float c4p;
            static float c5p;
            static float c6p;
            if (gfn::editor::onFocusDocument) {
                c1p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c1);
                c2p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c2);
                c3p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c3);
                c4p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c4);
                c5p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c5);
                c6p = float(gfn::editor::focusedDocument->interface.configs.getRead()->c6);
            }
            gfn::editor::focusedDocument->interface.configs.readDone();
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
                gfn::editor::execute("constant -c1 =" + std::to_string(c1));
            if (c2p != c2)
                gfn::editor::execute("constant -c2 =" + std::to_string(c2));
            if (c3p != c3)
                gfn::editor::execute("constant -c3 =" + std::to_string(c3));
            if (c4p != c4)
                gfn::editor::execute("constant -c4 =" + std::to_string(c4));
            if (c5p != c5)
                gfn::editor::execute("constant -c5 =" + std::to_string(c5));
            if (c6p != c6)
                gfn::editor::execute("constant -c6 =" + std::to_string(c6));
            c1p = c1;
            c2p = c2;
            c3p = c3;
            c4p = c4;
            c5p = c5;
            c6p = c6;
        }

        auto doc = gfn::editor::focusedDocument;
        if (doc) {
            if (ImGui::TreeNode("UserGraph adjacency list")) {
                if (ImGui::TreeNode("Vertices")) {
                    auto g = doc->interface.usergraph.getRead();
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
                    for (auto& v : doc->interface.userprops.getRead()->getVertexPropList()) {
                        if (ImGui::TreeNode(v.first.c_str())) {
                            ImGui::Text(("enabled: " + std::string(v.second.enabled ? "true" : "false")).c_str());
                            ImGui::TreePop();
                        }
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Edges")) {
                    for (auto& v : doc->interface.userprops.getRead()->getEdgePropList()) {
                        if (ImGui::TreeNode(v.first.c_str())) {
                            ImGui::Text(("enabled: " + std::string(v.second.enabled ? "true" : "false")).c_str());
                            ImGui::Text(("start: " + v.second.startVertexUuid).c_str());
                            ImGui::Text(("end: " + v.second.endVertexUuid).c_str());
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
        ImGui::End();

        gfn::window::postFrame();
    }

    gfn::editor::terminate();

    gfn::window::closeWindow();
    return 0;
}
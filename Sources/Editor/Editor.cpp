#include "Editor.h"
#include <Core/Objects/Random.h>
#include <Editor/Theme/Theme.h> /// TODO
#include <ImGuiFileDialog.h> /// TODO
#include <iostream>

namespace gfn {
    Editor::Editor() :
            hk(&prefs) {
        startTerminal();
        prefs.loadFromFile();
        gfx.launchWindow(&prefs);
        gfn::setTheme(&prefs);
        prefs.saveToFile();
        ImGuiFileDialog::Instance()->SetExtentionInfos(".gfn",
                                                       ImGui::ColorConvertU32ToFloat4(IM_COL32(255, 192, 66, 255)),
                                                       "\uea0d");
    }

    void Editor::update() {
        gfx.preFrame();

        hk.updateHotKeyState();

        if (!terminalBuffer.empty()) {
            if (gfn::isUuid(terminalBuffer.front())) {
                if (getDoc(terminalBuffer.front().substr(0, 36)))
                    getDoc(terminalBuffer.front().substr(0, 36))->execute(terminalBuffer.front().substr(37));
            } else {
                if (!getDoc(gfn::createNil()))
                    newDocument(gfn::createNil());
                getDoc(gfn::createNil())->execute(terminalBuffer.front());
            }
            terminalBuffer.pop();
        }

        auto fDoc = getDoc(activeDoc);

        bool actionNewDocument = hk.press(Actions::NEW_DOCUMENT, -1);
        bool actionOpenFile = hk.press(Actions::OPEN_FILE, -1);
        bool actionSaveFile = hk.press(Actions::SAVE_FILE, -1);
        bool actionSaveAsFile = hk.press(Actions::SAVE_AS_FILE, -1);
        bool actionCloseDocument = hk.press(Actions::CLOSE_DOCUMENT, -1);
        bool actionPreferences = hk.press(Actions::PREFERENCES, -1);
        bool actionKeyBindings = hk.press(Actions::KEY_BINDINGS, -1);
        bool actionQuit = hk.press(Actions::QUIT, -1);

        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("\ue24d New document", nullptr, false, true))
                actionNewDocument = true;
            if (ImGui::MenuItem("\ue2c7 Open file", nullptr, false, true))
                actionOpenFile = true;
            if (ImGui::MenuItem("\ue161 Save file", nullptr, false, fDoc))
                actionSaveFile = true;
            if (ImGui::MenuItem("\ue161 Save as file", nullptr, false, fDoc))
                actionSaveAsFile = true;
            if (ImGui::MenuItem("\ue5cd Close document", nullptr, false, fDoc))
                actionCloseDocument = true;
            ImGui::Separator();
            if (ImGui::MenuItem("\ue8b8 Preferences", nullptr, false, true))
                actionPreferences = true;
            if (ImGui::MenuItem("\ue312 Key bindings", nullptr, false, true))
                actionKeyBindings = true;
            ImGui::Separator();
            if (ImGui::MenuItem("\ue5cd Quit", nullptr, false))
                actionQuit = true;
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Insert")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Export")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

        if (actionNewDocument)
            newDocument();
        if (actionOpenFile)
            ImGuiFileDialog::Instance()->OpenDialog("OpenFile", "\ue2c7 Open file", ".gfn,.*", ".", 0, nullptr, 0);
        if (actionSaveFile && fDoc)
            fDoc->execute("save");
        if (actionSaveAsFile && fDoc) {
            ImGuiFileDialog::Instance()->OpenDialog("SaveAsFile", "\ue2c7 Save as file", ".gfn,.*", ".", 0, nullptr,
                                                    ImGuiFileDialogFlags_IsSave | ImGuiFileDialogFlags_ConfirmOverwrite);
        }
        if (actionCloseDocument && fDoc) {
            fDoc->showCloseConfirmationDialog = true;
            fDoc->closeDocument = true;
        }

        if (actionPreferences) {
            prefs.loadFromFile();
            prefs.showPrefsWindow = true;
        }
        if (actionKeyBindings) {
            prefs.loadFromFile();
            prefs.bindings.showBindingsConfigWindow = true;
        }
        if (actionQuit)
            glfwSetWindowShouldClose(gfx.glfwWindow, true);

        if (ImGuiFileDialog::Instance()->Display("OpenFile")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::replace(filePathName.begin(), filePathName.end(), '\\', '/');
                bool exists = false;
                for (auto& d : documents) {
                    if (d.second->file == filePathName) {
                        ImGui::SetWindowFocus((d.second->docName + "###" + d.first).c_str());
                        exists = true;
                        break;
                    }
                }
                if (!exists) {
                    auto docId = newDocument();
                    getDoc(docId)->docName = filePathName.substr(filePathName.find_last_of('/') + 1);
                    getDoc(docId)->setFile(filePathName);
                    getDoc(docId)->execute("open");
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }


        // FILE DIALOG
        if (ImGuiFileDialog::Instance()->Display("SaveAsFile")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                std::replace(filePathName.begin(), filePathName.end(), '\\', '/');

                fDoc->docName = filePathName.substr(filePathName.find_last_of('/') + 1);
                fDoc->setFile(filePathName);
                fDoc->execute("save");
            }
            ImGuiFileDialog::Instance()->Close();
        }


        ImGui::Begin("\ue30f Controls", nullptr, 0);
        if (fDoc) {
            if (gfn::button("\ue5d0 zoom to fit", HUE_YELLOW, HUE_DEFAULT, false, ImGui::GetContentRegionAvailWidth(), 0, false))
                fDoc->doZoomToFit = true;

            if (gfn::button("\ue6b8 export", HUE_CYAN, HUE_DEFAULT, false,
                            ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->showExportPopup = true;
            }

            gfn::Hue color = HUE_ORANGE_CONTRAST;
            std::string icon = "\uef55";
            if (!fDoc->isGraphUpdate) {
                color = HUE_RED_CONTRAST;
                icon = "\ue034";
            } else if (fDoc->getItf()->graph.getRead()->cfg.energySavingMode) {
                color = HUE_GREEN_CONTRAST;
                icon = "\uea35";
            }

            if (gfn::button(icon + " pause updates", color, color, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->isGraphUpdate = !fDoc->isGraphUpdate;
                fDoc->isGraphUpdateEx = true;
            }

            /*if (gfn::button("\ue51f pause graph streaming", fDoc->isGraphStreaming ? HUE_DEFAULT : HUE_BLUE_CONTRAST,
                            fDoc->isGraphStreaming ? HUE_DEFAULT : HUE_BLUE_CONTRAST,
                            false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->isGraphStreaming = !fDoc->isGraphStreaming;
            }*/

            if (gfn::button("\ue028 recalculate", HUE_RED, HUE_DEFAULT, false,
                            ImGui::GetContentRegionAvailWidth(), 0, false)) {
                for (auto& v:fDoc->getItf()->graph.getRead()->props.getVertexPropsList()) {
                    std::uniform_real_distribution dis(-20.0, 20.0);
                    auto randX = dis(gfn::getRng());
                    auto randY = dis(gfn::getRng());
                    fDoc->execute(
                            "setvertexprops -uuid=" + v.first + " -key=position -value=(" + std::to_string(randX) + "," + std::to_string(randY) + ")");
                }
            }

            ImGui::PushFont(gfx.fontSmall);
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 160, 160, 255));
            ImGui::TextWrapped(("DocID: " + fDoc->docId).c_str());
            ImGui::PopFont();
            ImGui::PopStyleColor(1);

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Copy to clipboard");
            if (ImGui::IsItemClicked())
                ImGui::SetClipboardText(fDoc->docId.c_str());
        }
        ImGui::End();


        ImGui::Begin("\ue9ef Constants");
        if (fDoc) {
            static float c1p;
            static float c2p;
            static float c3p;
            static float c4p;
            static float c5p;
            static float c6p;
            static float c7p;
            static float c8p;
            static float c9p;


            ImGui::BeginTabBar("##constantModes");
            if (ImGui::BeginTabItem("Basic")) {
                bool edit = false;
                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                static float preferredEdgeLength = 2.0f;
                ImGui::TextWrapped("Preferred edge length");

                ImGui::PushFont(gfx.fontSmall);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8.0f);
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 160, 160, 255));
                ImGui::TextWrapped("First, change this value so the edge length looks reasonable");
                ImGui::PopFont();
                ImGui::PopStyleColor(1);

                ImGui::DragFloat("##preferredEdgeLength", &preferredEdgeLength, 0.01f, 0.001f, FLT_MAX, "%.3f");
                if (ImGui::IsItemEdited()) edit = true;
                ImGui::Separator();

                static float repelForce = 1.0f;
                ImGui::TextWrapped("Repel force between vertices");

                ImGui::PushFont(gfx.fontSmall);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8.0f);
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 160, 160, 255));
                ImGui::TextWrapped("Then, increase this value if the graph is all crammed together");
                ImGui::PopFont();
                ImGui::PopStyleColor(1);

                ImGui::DragFloat("##repelForce", &repelForce, 0.01f, 0.001f, FLT_MAX, "%.3f");
                if (ImGui::IsItemEdited()) edit = true;
                ImGui::Separator();

                static float attractCoeff = 1.0f;
                ImGui::TextWrapped("Attract force coefficient");

                ImGui::PushFont(gfx.fontSmall);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 8.0f);
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(160, 160, 160, 255));
                ImGui::TextWrapped("If some edges become too long, increase this value");
                ImGui::PopFont();
                ImGui::PopStyleColor(1);

                ImGui::DragFloat("##attractCoeff", &attractCoeff, 0.01f, 0.001f, FLT_MAX, "%.3f");
                if (ImGui::IsItemEdited()) edit = true;
                ImGui::Separator();

                ImGui::PopItemWidth();

                if (edit) {
                    fDoc->execute("configs -c3=" + std::to_string(repelForce));
                    fDoc->execute("configs -c1=" + std::to_string(attractCoeff));
                    fDoc->execute("configs -c2=" +
                                  std::to_string(preferredEdgeLength / pow(10, (repelForce / (attractCoeff * preferredEdgeLength * preferredEdgeLength)))));
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Advanced")) {
                bool edit = false;
                ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                float c1 = float(fDoc->getItf()->graph.getRead()->cfg.c1.value);
                float c2 = float(fDoc->getItf()->graph.getRead()->cfg.c2.value);
                float c3 = float(fDoc->getItf()->graph.getRead()->cfg.c3.value);
                float c4 = float(fDoc->getItf()->graph.getRead()->cfg.c4.value);
                float c5 = float(fDoc->getItf()->graph.getRead()->cfg.c5.value);
                float c6 = float(fDoc->getItf()->graph.getRead()->cfg.c6.value);
                float c7 = float(fDoc->getItf()->graph.getRead()->cfg.c7.value);
                float c8 = float(fDoc->getItf()->graph.getRead()->cfg.c8.value);
                float c9 = float(fDoc->getItf()->graph.getRead()->cfg.c9.value);

                ImGui::Text("c1");
                ImGui::SliderFloat("##c1", &c1, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c1=" + std::to_string(c1));
                ImGui::Text("c2");
                ImGui::SliderFloat("##c2", &c2, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c2=" + std::to_string(c2));
                ImGui::Text("c3");
                ImGui::SliderFloat("##c3", &c3, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c3=" + std::to_string(c3));
                ImGui::Text("c4");
                ImGui::SliderFloat("##c4", &c4, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c4=" + std::to_string(c4));
                ImGui::Text("c5");
                ImGui::SliderFloat("##c5", &c5, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c5=" + std::to_string(c5));
                ImGui::Text("c6");
                ImGui::SliderFloat("##c6", &c6, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c6=" + std::to_string(c6));
                ImGui::Text("c7");
                ImGui::SliderFloat("##c7", &c7, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c7=" + std::to_string(c7));
                ImGui::Text("c8");
                ImGui::SliderFloat("##c8", &c8, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c8=" + std::to_string(c8));
                ImGui::Text("c9");
                ImGui::SliderFloat("##c9", &c9, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                if (ImGui::IsItemEdited())
                    fDoc->execute("configs -c9=" + std::to_string(c9));
                ImGui::PopItemWidth();

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();


        showPropertiesPanel();

        if (prefs.bindings.showBindingsConfigWindow) {
            ImGui::OpenPopup("\ue312 Key Binds##KEYBIND_ENROLL");
            prefs.bindings.showKeybindSetup();
        }
        if (prefs.showPrefsWindow) {
            ImGui::OpenPopup("\ue8b8 Preferences");
            prefs.preferencesPanel();
        }
        if (fDoc && fDoc->showExportPopup) {
            ImGui::OpenPopup("\ue6b8 Export");
            fDoc->exportMenu();
        }

        ImGui::Begin("\ue922 Timeline");
        if (fDoc) {
            static int frame = 0;
            int was = frame;
            bool update = false;
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 64.0f);
            ImGui::SliderInt("##Timeline", &frame, 0, 100, "%d", 0);
            if (ImGui::IsItemEdited()) update = true;
            ImGui::SameLine();
            if (ImGui::Button("\ue5c4")) {
                frame--;
                update = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("\ue5c8")) {
                frame++;
                update = true;
            }

            frame = std::max(0, std::min(100, frame));
            if (update) {
                //  fDoc->execute("save -f=\"timeline/" + std::to_string(was) + ".gfn");
                fDoc->setFile("timeline/" + std::to_string(frame) + ".gfn");
                fDoc->execute("open");
            }
        }
        ImGui::End();

        ImGui::Begin("\ue8ee Insert graph");
        static char insertGraph[65536];

        if (fDoc &&
            gfn::button("\ue147 Insert", HUE_CONTRAST, HUE_BLUE_CONTRAST, false, (ImGui::GetContentRegionAvailWidth() - ImGui::GetStyle().ItemSpacing.x) / 2, 0,
                        false)) {
            gfn::Uuid insertId = gfn::createUuid();
            int v, e;
            std::stringstream ss(insertGraph);
            ss >> v >> e;
            for (int i = 0; i < v; i++) {
                fDoc->execute("mkvertex -name=" + insertId + ":" + std::to_string(i));
                fDoc->execute("setvertexprops -name=" + insertId + ":" + std::to_string(i) + " -key=label -value=" + std::to_string(i));
            }
            for (int i = 0; i < e; i++) {
                int a, b;
                ss >> a >> b;
                fDoc->execute("mkedge -uname=" + insertId + ":" + std::to_string(a) + " -vname=" + insertId + ":" + std::to_string(b));
            }
        }
        ImGui::SameLine();
        if (gfn::button("\ue872 Clear", HUE_RED, HUE_DEFAULT, false, ImGui::GetContentRegionAvailWidth(), 0, false))
            insertGraph[0] = '\0';

        ImGui::InputTextMultiline("##insertGraph", insertGraph, 65536, ImGui::GetContentRegionAvail());

        ImGui::End();

        updateDocuments();

        prefs.checkSave();

        gfx.postFrame();
    }

    void Editor::updateDocuments() {
        // document close handling
        // the close confirmation and state is handled by the document itself
        for (auto dIt = documents.begin(); dIt != documents.end();) {
            if (dIt->second->closeDocument) {
                if (activeDoc == dIt->second->docId)
                    activeDoc = "";
                closeDocument(dIt->first);
                dIt = documents.begin(); // save the hassle
            } else dIt++;
        }

        // document focus & save as handling (untitled documents can not save as themselves)
        for (auto& d : documents) {
            if (d.second->isFocused)
                activeDoc = d.second->docId;
        }

        for (auto& d : documents) {
            d.second->update();
        }
    }

    bool Editor::terminateTerminal = false;
    std::queue<std::string> Editor::terminalBuffer;

    void Editor::startTerminal() {
        std::thread terminalThread([]() {
            while (!terminateTerminal) {
                std::string cmd;
                std::getline(std::cin, cmd);
                if (cmd == "exit")
                    break;
                Editor::execute(cmd);
            }
        });
        terminalThread.detach();
    }

    void Editor::execute(const std::string& cmd) {
        terminalBuffer.push(cmd);
    }

    void Editor::terminate() {
        for (auto& d : documents)
            d.second->terminate();
        for (auto& d : documents)
            delete d.second;
        gfx.closeWindow();
    }

}
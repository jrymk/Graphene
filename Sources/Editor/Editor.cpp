#include "Editor.h"
#include <Core/Objects/Random.h>
#include <Editor/Theme/Theme.h> /// TODO
#include <ImGuiFileDialog.h> /// TODO
#include <iostream>
#include <Tracy.hpp>

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

        ImGui::Begin("\ue30f Controls", nullptr, 0);


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


        if (fDoc) {
            if (gfn::button("\ue034 pause updates", HUE_CONTRAST,
                            fDoc->isGraphUpdate ? HUE_CONTRAST : HUE_RED_CONTRAST, false,
                            ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->isGraphUpdate = !fDoc->isGraphUpdate;
                fDoc->isGraphUpdateEx = true;
            }
            if (gfn::button("\ue51f pause graph streaming", HUE_CONTRAST, fDoc->isGraphStreaming ? HUE_CONTRAST : HUE_BLUE_CONTRAST,
                            false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->isGraphStreaming = !fDoc->isGraphStreaming;
            }
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

            if (gfn::button("\ue6b8 export", HUE_CYAN, HUE_DEFAULT, false,
                            ImGui::GetContentRegionAvailWidth(), 0, false)) {
                fDoc->showExportPopup = true;
            }

            ImGui::Text(("DocID: " + fDoc->docId).c_str());
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Copy to clipboard");
            if (ImGui::IsItemClicked())
                ImGui::SetClipboardText(fDoc->docId.c_str());
        }

        ImGui::Separator();

        ImGui::Text("\ue9ef Constants");
        static gfn::Uuid prevActiveDocId;
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
//if (prevActiveDocId != fDoc->docId) {
            prevActiveDocId = fDoc->docId;
            c1p = float(fDoc->getItf()->graph.getRead()->cfg.c1.value);
            c2p = float(fDoc->getItf()->graph.getRead()->cfg.c2.value);
            c3p = float(fDoc->getItf()->graph.getRead()->cfg.c3.value);
            c4p = float(fDoc->getItf()->graph.getRead()->cfg.c4.value);
            c5p = float(fDoc->getItf()->graph.getRead()->cfg.c5.value);
            c6p = float(fDoc->getItf()->graph.getRead()->cfg.c6.value);
            c7p = float(fDoc->getItf()->graph.getRead()->cfg.c7.value);
            c8p = float(fDoc->getItf()->graph.getRead()->cfg.c8.value);
            c9p = float(fDoc->getItf()->graph.getRead()->cfg.c9.value);
//}
            float c1 = c1p;
            float c2 = c2p;
            float c3 = c3p;
            float c4 = c4p;
            float c5 = c5p;
            float c6 = c6p;
            float c7 = c7p;
            float c8 = c8p;
            float c9 = c9p;
            if (ImGui::BeginTable("##CONSTANTS", 2, ImGuiTableFlags_BordersH | ImGuiTableFlags_Resizable)) {
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c1");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c1", &c1, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c2");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c2", &c2, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c3");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c3", &c3, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c4");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c4", &c4, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c5");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c5", &c5, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c6");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c6", &c6, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c7");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c7", &c7, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c8");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c8", &c8, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("c9");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
                    ImGui::SliderFloat("##c9", &c9, 0.000001, 1000.0, "%f", ImGuiSliderFlags_Logarithmic);
                    ImGui::PopItemWidth();
                }
                ImGui::EndTable();
            }
            if (c1p != c1)
                fDoc->execute("configs -c1=" + std::to_string(c1));
            if (c2p != c2)
                fDoc->execute("configs -c2=" + std::to_string(c2));
            if (c3p != c3)
                fDoc->execute("configs -c3=" + std::to_string(c3));
            if (c4p != c4)
                fDoc->execute("configs -c4=" + std::to_string(c4));
            if (c5p != c5)
                fDoc->execute("configs -c5=" + std::to_string(c5));
            if (c6p != c6)
                fDoc->execute("configs -c6=" + std::to_string(c6));
            if (c7p != c7)
                fDoc->execute("configs -c7=" + std::to_string(c7));
            if (c8p != c8)
                fDoc->execute("configs -c8=" + std::to_string(c8));
            if (c9p != c9)
                fDoc->execute("configs -c9=" + std::to_string(c9));
            c1p = c1;
            c2p = c2;
            c3p = c3;
            c4p = c4;
            c5p = c5;
            c6p = c6;
            c7p = c7;
            c8p = c8;
            c9p = c9;
        }

        ImGui::Separator();

        if (fDoc) {
            if (gfn::button("\ue5d0 zoom to fit", HUE_YELLOW, HUE_DEFAULT, false, ImGui::GetContentRegionAvailWidth(), 0, false))
                fDoc->doZoomToFit = true;
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


        updateDocuments();

        prefs.checkSave();

        gfx.postFrame();

        FrameMark
    }

    void Editor::updateDocuments() {
        ZoneScoped
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
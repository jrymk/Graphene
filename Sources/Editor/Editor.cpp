#include "Editor.h"
#include <Editor/Theme/Theme.h> /// TODO
#include <ImGuiFileBrowser.h> /// TODO
#include <Tracy.hpp>

namespace gfn {
    Editor::Editor() :
            hk(&prefs) {
        //cmdStartup();
        prefs.loadFromFile();
        gfx.launchWindow(&prefs);
        gfn::setTheme(&prefs);
        prefs.saveToFile();
    }

    void Editor::update() {
        gfx.preFrame();

        hk.updateHotKeyState();

        //gfn::setColorLight();
        /*ImGui::Begin("LIGHT THEME");
        gfn::text("BORDER ON", (gfn::Hue) HUE_DEFAULT);
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 17; j++) {
                int text =
                        (i == 0 ? HUE_DEFAULT :
                         (i == 1 ? HUE_CONTRAST :
                          (i == 2 ? HUE_TRANSPARENT :
                           (i == 3 ? HUE_RED :
                            (i == 4 ? HUE_ORANGE :
                             (i == 5 ? HUE_YELLOW :
                              (i == 6 ? HUE_GREEN :
                               (i == 7 ? HUE_CYAN :
                                (i == 8 ? HUE_BLUE :
                                 (i == 9 ? HUE_PURPLE :
                                  (i == 10 ? HUE_RED_CONTRAST :
                                   (i == 11 ? HUE_ORANGE_CONTRAST :
                                    (i == 12 ? HUE_YELLOW_CONTRAST :
                                     (i == 13 ? HUE_GREEN_CONTRAST :
                                      (i == 14 ? HUE_CYAN_CONTRAST :
                                       (i == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                int button =
                        (j == 0 ? HUE_DEFAULT :
                         (j == 1 ? HUE_CONTRAST :
                          (j == 2 ? HUE_TRANSPARENT :
                           (j == 3 ? HUE_RED :
                            (j == 4 ? HUE_ORANGE :
                             (j == 5 ? HUE_YELLOW :
                              (j == 6 ? HUE_GREEN :
                               (j == 7 ? HUE_CYAN :
                                (j == 8 ? HUE_BLUE :
                                 (j == 9 ? HUE_PURPLE :
                                  (j == 10 ? HUE_RED_CONTRAST :
                                   (j == 11 ? HUE_ORANGE_CONTRAST :
                                    (j == 12 ? HUE_YELLOW_CONTRAST :
                                     (j == 13 ? HUE_GREEN_CONTRAST :
                                      (j == 14 ? HUE_CYAN_CONTRAST :
                                       (j == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                gfn::button("Theme", (gfn::Hue) text, (gfn::Hue) button, true, 0, 0);
                if (j != 16)
                    ImGui::SameLine();
            }
        }
        gfn::text("BORDER OFF", (gfn::Hue) HUE_DEFAULT);
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 17; j++) {
                int text =
                        (i == 0 ? HUE_DEFAULT :
                         (i == 1 ? HUE_CONTRAST :
                          (i == 2 ? HUE_TRANSPARENT :
                           (i == 3 ? HUE_RED :
                            (i == 4 ? HUE_ORANGE :
                             (i == 5 ? HUE_YELLOW :
                              (i == 6 ? HUE_GREEN :
                               (i == 7 ? HUE_CYAN :
                                (i == 8 ? HUE_BLUE :
                                 (i == 9 ? HUE_PURPLE :
                                  (i == 10 ? HUE_RED_CONTRAST :
                                   (i == 11 ? HUE_ORANGE_CONTRAST :
                                    (i == 12 ? HUE_YELLOW_CONTRAST :
                                     (i == 13 ? HUE_GREEN_CONTRAST :
                                      (i == 14 ? HUE_CYAN_CONTRAST :
                                       (i == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                int button =
                        (j == 0 ? HUE_DEFAULT :
                         (j == 1 ? HUE_CONTRAST :
                          (j == 2 ? HUE_TRANSPARENT :
                           (j == 3 ? HUE_RED :
                            (j == 4 ? HUE_ORANGE :
                             (j == 5 ? HUE_YELLOW :
                              (j == 6 ? HUE_GREEN :
                               (j == 7 ? HUE_CYAN :
                                (j == 8 ? HUE_BLUE :
                                 (j == 9 ? HUE_PURPLE :
                                  (j == 10 ? HUE_RED_CONTRAST :
                                   (j == 11 ? HUE_ORANGE_CONTRAST :
                                    (j == 12 ? HUE_YELLOW_CONTRAST :
                                     (j == 13 ? HUE_GREEN_CONTRAST :
                                      (j == 14 ? HUE_CYAN_CONTRAST :
                                       (j == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                gfn::button("Theme", (gfn::Hue) text, (gfn::Hue) button, false, 0, 0);
                if (j != 16)
                    ImGui::SameLine();
            }
        }
        ImGui::End();

        //gfn::setColorDark();
        ImGui::Begin("DARK THEME");
        gfn::text("BORDER ON", (gfn::Hue) HUE_DEFAULT);
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 17; j++) {
                int text =
                        (i == 0 ? HUE_DEFAULT :
                         (i == 1 ? HUE_CONTRAST :
                          (i == 2 ? HUE_TRANSPARENT :
                           (i == 3 ? HUE_RED :
                            (i == 4 ? HUE_ORANGE :
                             (i == 5 ? HUE_YELLOW :
                              (i == 6 ? HUE_GREEN :
                               (i == 7 ? HUE_CYAN :
                                (i == 8 ? HUE_BLUE :
                                 (i == 9 ? HUE_PURPLE :
                                  (i == 10 ? HUE_RED_CONTRAST :
                                   (i == 11 ? HUE_ORANGE_CONTRAST :
                                    (i == 12 ? HUE_YELLOW_CONTRAST :
                                     (i == 13 ? HUE_GREEN_CONTRAST :
                                      (i == 14 ? HUE_CYAN_CONTRAST :
                                       (i == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                int button =
                        (j == 0 ? HUE_DEFAULT :
                         (j == 1 ? HUE_CONTRAST :
                          (j == 2 ? HUE_TRANSPARENT :
                           (j == 3 ? HUE_RED :
                            (j == 4 ? HUE_ORANGE :
                             (j == 5 ? HUE_YELLOW :
                              (j == 6 ? HUE_GREEN :
                               (j == 7 ? HUE_CYAN :
                                (j == 8 ? HUE_BLUE :
                                 (j == 9 ? HUE_PURPLE :
                                  (j == 10 ? HUE_RED_CONTRAST :
                                   (j == 11 ? HUE_ORANGE_CONTRAST :
                                    (j == 12 ? HUE_YELLOW_CONTRAST :
                                     (j == 13 ? HUE_GREEN_CONTRAST :
                                      (j == 14 ? HUE_CYAN_CONTRAST :
                                       (j == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                gfn::button("Theme", (gfn::Hue) text, (gfn::Hue) button, true, 0, 0);
                if (j != 16)
                    ImGui::SameLine();
            }
        }
        gfn::text("BORDER OFF", (gfn::Hue) HUE_DEFAULT);
        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 17; j++) {
                int text =
                        (i == 0 ? HUE_DEFAULT :
                         (i == 1 ? HUE_CONTRAST :
                          (i == 2 ? HUE_TRANSPARENT :
                           (i == 3 ? HUE_RED :
                            (i == 4 ? HUE_ORANGE :
                             (i == 5 ? HUE_YELLOW :
                              (i == 6 ? HUE_GREEN :
                               (i == 7 ? HUE_CYAN :
                                (i == 8 ? HUE_BLUE :
                                 (i == 9 ? HUE_PURPLE :
                                  (i == 10 ? HUE_RED_CONTRAST :
                                   (i == 11 ? HUE_ORANGE_CONTRAST :
                                    (i == 12 ? HUE_YELLOW_CONTRAST :
                                     (i == 13 ? HUE_GREEN_CONTRAST :
                                      (i == 14 ? HUE_CYAN_CONTRAST :
                                       (i == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                int button =
                        (j == 0 ? HUE_DEFAULT :
                         (j == 1 ? HUE_CONTRAST :
                          (j == 2 ? HUE_TRANSPARENT :
                           (j == 3 ? HUE_RED :
                            (j == 4 ? HUE_ORANGE :
                             (j == 5 ? HUE_YELLOW :
                              (j == 6 ? HUE_GREEN :
                               (j == 7 ? HUE_CYAN :
                                (j == 8 ? HUE_BLUE :
                                 (j == 9 ? HUE_PURPLE :
                                  (j == 10 ? HUE_RED_CONTRAST :
                                   (j == 11 ? HUE_ORANGE_CONTRAST :
                                    (j == 12 ? HUE_YELLOW_CONTRAST :
                                     (j == 13 ? HUE_GREEN_CONTRAST :
                                      (j == 14 ? HUE_CYAN_CONTRAST :
                                       (j == 15 ? HUE_BLUE_CONTRAST :
                                        HUE_PURPLE_CONTRAST))))))))))))))));
                gfn::button("Theme", (gfn::Hue) text, (gfn::Hue) button, false, 0, 0);
                if (j != 16)
                    ImGui::SameLine();
            }
        }
        ImGui::End();

        //gfn::setColorDark();*/

        //parseCommandQueue();


        //        static bool first = true;
        //        if (first) {
        //            gfn::loadDragAndDrop(argc, argv, true);
        //            first = false;
        //        }
        //
        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();

        /*auto fDoc = gfn::getActiveDocument();*/

        ImGui::Begin("Hot keys 2.0");
        if (getDoc(activeDoc)) {
            gfn::text(getDoc(activeDoc)->docName + ": " + std::to_string(getDoc(activeDoc)->graphview.camera.hoverState), HUE_DEFAULT);
            for (int i = 0; i < gfn::Bindings::actions.size(); i++) {
                if (hk.timeSincePressMs(i, getDoc(activeDoc)->graphview.camera.hoverState) < 100)
                    gfn::text(gfn::Bindings::actions[i].first + " PRESSED", HUE_RED);
                if (hk.down(i, getDoc(activeDoc)->graphview.camera.hoverState))
                    gfn::text(gfn::Bindings::actions[i].first + " DOWN", HUE_YELLOW);
                if (hk.timeSinceReleaseMs(i, getDoc(activeDoc)->graphview.camera.hoverState) < 100)
                    gfn::text(gfn::Bindings::actions[i].first + " RELEASED", HUE_BLUE);
            }
        } else
            gfn::text("No active document", HUE_RED);

        ImGui::End();

        ImGui::Begin("Args centre", nullptr, 0);

        if (ImGui::Button("\ue24d New file"))
            newDocument();
        ImGui::SameLine();

        static bool opening = false;
        if (ImGui::Button("\ue2c7 Open file")) {
            opening = true;
            ImGui::OpenPopup("Open File");
        }

        if (getDoc(activeDoc)) {
            ImGui::SameLine();
            if (ImGui::Button("\ue161 Save file"))
                getDoc(activeDoc)->execute("save");
        }

        static imgui_addons::ImGuiFileBrowser fileDialog;
        if (fileDialog.showFileDialog("\ue2c7 Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                      ImVec2(700, 310), ".gfn")) {
            opening = false;
            auto path = fileDialog.selected_path;
            for (auto& d : documents) {
                if (d.second->file == fileDialog.selected_path) {
                    ImGui::SetWindowFocus((d.second->docName + "###" + d.first).c_str());
                    return;
                }
            }
            auto docId = newDocument();
            getDoc(docId)->docName = path.substr(path.find_last_of('/') + 1);
            getDoc(docId)->setFile(path);
            //getDocumentFromUuid(docId)->fileSaved = true;
            getDoc(docId)->execute("open");
        }
        /*if (fileDialog.showFileDialog("Save As File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE,
                                      ImVec2(700, 310), ".gfn")) {
            isSavingAsFile = false;
            execute("select -uuid=" + saveAsFileId);
            execute("save -f=\"" + fileDialog.selected_path + "\"");
            auto path = fileDialog.selected_path;
            getDocumentFromUuid(saveAsFileId)->displayName = path.substr(path.find_last_of('/') + 1);
            getDocumentFromUuid(saveAsFileId)->filePath = path;
            //getDocumentFromUuid(docId)->fileSaved = true;
            execute("open -f=\"" + fileDialog.selected_path + "\"");
            if (getDocumentFromUuid(saveAsFileId)->wantSaveAsAndClose)
                getDocumentFromUuid(saveAsFileId)->closeDocument = true;
        }*/








        /*if (!gfn::activeDocumentUuid.empty()) {
            ImGui::SameLine();
            if (gfn::getActiveDocument()->file.empty()) {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 200, 200, 255));
            }
            if (ImGui::Button("Save File"))
                gfn::saveFile();
            if (gfn::getActiveDocument()->file.empty()) {
                ImGui::PopItemFlag();
                ImGui::PopStyleColor(1);
            }
            ImGui::SameLine();
            if (ImGui::Button("Save As File"))
                gfn::saveAsFile(gfn::activeDocumentUuid);
        }*/

        ImGui::Separator();

        if (gfn::button("\ue312 Key binds", HUE_CONTRAST, HUE_CONTRAST, false, 0, 0, false)) {
            prefs.bindings.showBindingsConfigWindow = true;
            ImGui::OpenPopup("\ue312 Key Binds");
        }
        if (prefs.bindings.showBindingsConfigWindow)
            prefs.bindings.showKeybindSetup();

        ImGui::Separator();

        gfn::Document* fDoc = getDoc(activeDoc);

        if (fDoc) {
            //if (gfn::isOpeningFile)
            ImGui::Text(fDoc->docName.c_str());
            ImGui::Text("Documents:");
            //if (gfn::isSavingAsFile)
            for (auto& d : documents) {
                ImGui::Text(d.first.c_str());
                if (d.second->isFocused) {
                    ImGui::SameLine(300.0f);
                    ImGui::Text("focused");
                    ImGui::SameLine(380.0f);
                    ImGui::Text(d.second->getFileName().c_str());
                }
            }
        }

        ImGui::Separator();

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
            c1p = float(fDoc->getItf()->graph.getRead()->cfg.c1.value);
            c2p = float(fDoc->getItf()->graph.getRead()->cfg.c2.value);
            c3p = float(fDoc->getItf()->graph.getRead()->cfg.c3.value);
            c4p = float(fDoc->getItf()->graph.getRead()->cfg.c4.value);
            c5p = float(fDoc->getItf()->graph.getRead()->cfg.c5.value);
            c6p = float(fDoc->getItf()->graph.getRead()->cfg.c6.value);
            //}
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
            c1p = c1;
            c2p = c2;
            c3p = c3;
            c4p = c4;
            c5p = c5;
            c6p = c6;
        }

        ImGui::End();


        //gfn::showPropertiesPanel(); // must go before updateDocuments

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

    void Editor::terminate() {
        for (auto& d : documents)
            d.second->terminate();
        for (auto& d : documents)
            delete d.second;
        gfx.closeWindow();
    }
}
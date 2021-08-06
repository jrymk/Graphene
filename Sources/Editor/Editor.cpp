#include "Editor.h"
#include <Editor/Theme/Theme.hpp> /// TODO

namespace gfn {
    Editor::Editor() :
            hk(&prefs) {
        //cmdStartup();
        prefs.loadFromFile();
        gfx.launchWindow(&prefs);
        gfn::theme::setTheme(); /// TODO
    }

    void Editor::update() {
        gfx.preFrame();
        hk.update();

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

        ImGui::Begin("Args centre", nullptr, 0);

        if (ImGui::Button("New file"))
            newDocument();
        ImGui::SameLine();

        /*if (ImGui::Button("Open file"))
            gfn::openFile();
        if (!gfn::activeDocumentUuid.empty()) {
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

        if (ImGui::Button("Key binds")) {
            prefs.bindings.showBindingsConfigWindow = true;
            ImGui::OpenPopup("Key Binds");
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

    void Editor::terminate() {
        for (auto& d : documents)
            d.second->terminate();
        for (auto& d : documents)
            delete d.second;
        gfx.closeWindow();
    }
}
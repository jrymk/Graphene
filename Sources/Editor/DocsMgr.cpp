#include "Editor.h"

namespace gfn {
    gfn::Document* Editor::getDoc(const gfn::Uuid& docId) {
        auto dIt = documents.find(docId);
        if (dIt == documents.end())
            return nullptr;
        else
            return dIt->second;
    }

    gfn::Uuid Editor::newDocument() {
        gfn::Uuid docId = gfn::createUuid();
        auto doc =  new Document(docId, &hk, &prefs, &gfx);
        documents.insert({docId, doc});
        gfx.dockBuildWindow.push(doc->docName + "###" + docId);
        return docId;
    }

    void Editor::closeDocument(const gfn::Uuid& docId) {
        auto dIt = documents.find(docId);
        if (dIt == documents.end())
            return;
        dIt->second->terminate();
        delete dIt->second;
        documents.erase(dIt);
    }

    void Editor::loadDocsFromArgs(int argc, char* argv[], bool doNotCreateUntitled) {
        for (int f = 1; f < argc; f++) {
            std::string arg(argv[f]);
            std::replace(arg.begin(), arg.end(), '\\', '/');
            if (arg.substr(arg.find_last_of('.')) == ".gfn") {
                auto docId = newDocument();
                getDoc(docId)->execute("open -f=" + arg);
            }
        }
        // if no files are dragged into the executable, open an untitled document
        if (documents.empty() && !doNotCreateUntitled)
            newDocument();
    }


//    void Editor::updateFileDialog() {
//        static imgui_addons::ImGuiFileBrowser fileDialog;
//        if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
//                                      ImVec2(700, 310), ".gfn")) {
//            isOpeningFile = false;
//            auto path = fileDialog.selected_path;
//            for (auto& d : documents) {
//                if (d.second->file == fileDialog.selected_path) {
//                    ImGui::SetWindowFocus((d.second->docName + "###" + d.first).c_str());
//                    return;
//                }
//            }
//            auto docId = newFile();
//            getDocumentFromUuid(docId)->docName = path.substr(path.find_last_of('/') + 1);
//            getDocumentFromUuid(docId)->file = path;
//            //getDocumentFromUuid(docId)->fileSaved = true;
//            execute("select -uuid=" + docId);
//            execute("open -f=\"" + path + "\"");
//        }
//        if (fileDialog.showFileDialog("Save As File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE,
//                                      ImVec2(700, 310), ".gfn")) {
//            isSavingAsFile = false;
//            execute("select -uuid=" + saveAsFileId);
//            execute("save -f=\"" + fileDialog.selected_path + "\"");
//
//            auto path = fileDialog.selected_path;
//            getDocumentFromUuid(saveAsFileId)->docName = path.substr(path.find_last_of('/') + 1);
//            getDocumentFromUuid(saveAsFileId)->file = path;
//            //getDocumentFromUuid(docId)->fileSaved = true;
//            execute("open -f=\"" + fileDialog.selected_path + "\"");
//
//            if (getDocumentFromUuid(saveAsFileId)->wantSaveAsAndClose)
//                getDocumentFromUuid(saveAsFileId)->closeDocument = true;
//        }
//    }

}

#pragma once

#include <unordered_map>
#include <Document/Document.hpp>
#include <Objects/Uuid.hpp>
#include <ImGuiFileBrowser.h>

namespace gfn::editor {
    extern void execute(const std::string& cmd);
    extern gfn::preferences::Preferences preferences;

    std::unordered_map<gfn::Uuid, gfn::document::Document*> documents;

    gfn::Uuid activeDocumentUuid;

    gfn::document::Document* getActiveDocument() {
        auto dIt = documents.find(activeDocumentUuid);
        if (dIt == documents.end())
            return nullptr;
        else
            return dIt->second;
    }

    gfn::document::Document* getDocumentFromUuid(const gfn::Uuid& docId) {
        auto dIt = documents.find(docId);
        if (dIt == documents.end())
            return nullptr;
        else
            return dIt->second;
    }

    bool _onFocusDocument = false; // whether a document is focused on this frame

    void updateDocManagement() {
        // document focus
        _onFocusDocument = false;
        for (auto& d : documents) {
            if (d.second->isFocused && activeDocumentUuid != d.second->docId) {
                activeDocumentUuid = d.second->docId;
                _onFocusDocument = true;
            }
            if (getActiveDocument())
                getActiveDocument()->isFocused = true;
        }

        // document close handling
        // the close confirmation and state is handled by the document itself
        for (auto dIt = documents.begin(); dIt != documents.end();) {
            if (dIt->second->closeDocument) {
                if (activeDocumentUuid == dIt->second->docId)
                    activeDocumentUuid = "";
                dIt = documents.erase(dIt);
            } else dIt++;
        }
    }


    /// FILE SAVE LOAD
    bool isOpeningFile = false;
    gfn::Uuid saveAsFileId;
    bool isSavingAsFile = false;
    int untitledCounter = 1; // based on session

    gfn::Uuid newFile() {
        auto docId = gfn::uuid::createUuid();
        auto document = new gfn::document::Document("Untitled (" + std::to_string(untitledCounter) + ")",
                                                    docId, &preferences);
        untitledCounter++;
        documents.insert({docId, document});
        return docId;
    }

    void openFile() { isOpeningFile = true; }

    void openFileWithPath(const std::string& path) {
        auto docId = newFile();
        getDocumentFromUuid(docId)->displayName = path.substr(path.find_last_of('/') + 1);
        getDocumentFromUuid(docId)->filePath = path;
        execute("select -uuid=" + docId);
        execute("open -f=\"" + path + "\"");
    }

    void saveFile() {
        execute("select -uuid=" + activeDocumentUuid);
        execute("save -f=\"" + getActiveDocument()->filePath + "\"");
    }

    void saveAsFile(const gfn::Uuid& docId) {
        saveAsFileId = docId;
        isSavingAsFile = true;
    }

    void updateFileDialog() {
        static imgui_addons::ImGuiFileBrowser fileDialog;
        if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN,
                                      ImVec2(700, 310), ".gfn")) {
            isOpeningFile = false;
            auto docId = newFile();
            auto path = fileDialog.selected_path;
            getDocumentFromUuid(docId)->displayName = path.substr(path.find_last_of('/') + 1);
            getDocumentFromUuid(docId)->filePath = path;
            execute("select -uuid=" + docId);
            execute("open -f=\"" + path + "\"");
        }
        if (fileDialog.showFileDialog("Save As File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE,
                                      ImVec2(700, 310), ".gfn")) {
            isSavingAsFile = false;
            auto docId = newFile();
            auto path = fileDialog.selected_path;
            getDocumentFromUuid(docId)->displayName = path.substr(path.find_last_of('/') + 1);
            getDocumentFromUuid(docId)->filePath = path;
            execute("select -uuid=" + docId);
            execute("save -f=\"" + path + "\"");
        }
    }

    void terminate() {
        for (auto& d : documents)
            d.second->terminate();
    }
}
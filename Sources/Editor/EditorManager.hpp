#pragma once

#include <unordered_set>
#include <thread>
#include <iostream>
#include <string>
#include <iomanip>
#include <queue>
#include <sstream>
#include <Document/Document.hpp>
#include <binn.h>
#include <ImGuiFileBrowser.h>
#include <Preferences/Preferences.hpp>

/// A place where most of the GUI logic is
namespace gfn::editor {
    // one single preference object for the application
    gfn::preferences::Preferences preferences;

    /// DOCUMENT MANAGEMENT
    std::unordered_map<gfn::Uuid, gfn::document::Document*> documents;

    gfn::document::Document* docFind(const gfn::Uuid& docId) {
        if (documents.find(docId) == documents.end()) {
            //std::cerr << "Document ID: " << docId << " does not exist\n";
            return nullptr;
        }
        return documents.find(docId)->second;
    }

    gfn::Uuid focusedDocument;
    bool onFocusDocument = false; // whether a document is focused in this frame

    void updateDocManagement() {
        // document focus
        onFocusDocument = false;
        for (auto& d : documents) {
            if (d.second->isFocused && focusedDocument != d.second->docId) {
                focusedDocument = d.second->docId;
                onFocusDocument = true;
            }
            if (docFind(focusedDocument))
                docFind(focusedDocument)->isFocused = true;
        }
        // document close handling
        // the close confirmation and state is handled by the document itself
        for (auto dIt = documents.begin(); dIt != documents.end();) {
            if (dIt->second->closeDocument) {
                if (focusedDocument == dIt->second->docId)
                    focusedDocument = "";
                dIt = documents.erase(dIt);
            } else dIt++;
        }
    }
    /// DOCUMENT MANAGEMENT

    /// COMMAND INPUT
    bool stopInput = false;
    std::queue<std::string> commandQueue;

    void execute(const std::string& cmd) { commandQueue.push(cmd); }

    // continiously obtain command input from the terminal in the background
    void terminalInputLoop() {
        while (true) {
            std::string input;
            if (stopInput)
                break;
            std::getline(std::cin, input);
            commandQueue.push(input);
        }
    }

    void startup() {
        std::thread inputThread(&terminalInputLoop);
        inputThread.detach();
    }

    // parse select command that focuses document via command line
    void parseSelect(gfn::Command command, gfn::Command& output) {
        if (!command.getParamValue("-f").empty()) {
            std::string file = command.getParamValue("-f");
            int match = 0;
            gfn::Uuid matchDoc;
            for (auto& d : documents) {
                if (d.second->filePath == file) {
                    matchDoc = d.first;
                    break;
                }
                if (d.second->getFileName() == file) {
                    matchDoc = d.first;
                    match++;
                }
            }
            if (matchDoc.empty()) {
                output.newParam("-output", "Specified file name not found or not opened");
                output.newParam("-error", "FILE_NOT_FOUND");
            } else if (match > 1) {
                output.newParam("-output", "Multiple open files found with the same file name, "
                                           "try specify the full path");
                output.newParam("-error", "MULTIPLE_FILE_CANDADATES");
            } else
                focusedDocument = matchDoc;
        } else if (!command.getParamValue("-uuid").empty()) {
            if (documents.find(command.getParamValue("-uuid")) != documents.end())
                focusedDocument = command.getParamValue("-uuid");
        } else {
            output.newParam("-output", "Expected file name [-f] or document uuid [-uuid]");
            output.newParam("-error", "FILE_NOT_SPECIFIED");
        }
    }

    void parseCommandQueue() {
        while (!commandQueue.empty()) {
            std::string fullCmd = commandQueue.front();
            gfn::Command command(fullCmd), output;
            commandQueue.pop();
            std::string cmd = command.getParamValue("command");
            std::cout << fullCmd << "\n";
            if (cmd == "select") parseSelect(command, output);
            else if (docFind(focusedDocument)) docFind(focusedDocument)->execute(fullCmd);
            else output.newParam("-error", "NO_DOCUMENT_FOCUSED");

            if (output.getParamValue("successful") == "false" || output.getFlag("-error") || output.getFlag("-warning"))
                std::cerr << output.getString() << "\n";
            else
                std::cout << output.getString() << "\n";
        }
    }
    /// COMMAND INPUT

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
        docFind(docId)->displayName = path.substr(path.find_last_of('/') + 1);
        docFind(docId)->filePath = path;
        execute("select -uuid=" + docId);
        execute("open -f=\"" + path + "\"");
    }

    void saveFile() {
        execute("select -uuid=" + focusedDocument);
        execute("save -f=\"" + docFind(focusedDocument)->filePath + "\"");
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
            docFind(docId)->displayName = path.substr(path.find_last_of('/') + 1);
            docFind(docId)->filePath = path;
            execute("select -uuid=" + docId);
            execute("open -f=\"" + path + "\"");
        }
        if (fileDialog.showFileDialog("Save As File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE,
                                      ImVec2(700, 310), ".gfn")) {
            isSavingAsFile = false;
            auto docId = newFile();
            auto path = fileDialog.selected_path;
            docFind(docId)->displayName = path.substr(path.find_last_of('/') + 1);
            docFind(docId)->filePath = path;
            execute("select -uuid=" + docId);
            execute("save -f=\"" + path + "\"");
        }
    }
    /// FILE SAVE LOAD

    /// MAIN UPDATE FUNCTION
    void update() {
        updateFileDialog();

        for (auto &d : documents)
            d.second->update();

        updateDocManagement();

        parseCommandQueue();
    }
    /// MAIN UPDATE FUNCTION

    void terminate() {
        for (auto& d : documents)
            d.second->terminate();
    }

}
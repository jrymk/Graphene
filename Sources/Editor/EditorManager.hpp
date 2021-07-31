#pragma once

#include <unordered_set>
#include <thread>
#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <Document/Document.hpp>

namespace gfn::editor {
    // one single preference object for the application
    gfn::preferences::Preferences preferences;

    std::unordered_set<gfn::document::Document*> documents;
    gfn::document::Document* focusedDocument = nullptr;
    bool onFocusDocument = false;

    bool stopInput = false;

    bool waitingSerialized = false;
    gfn::Uuid serializeToken = "";
    gfn::document::Document* waitingSerializedDoc = nullptr;

    std::queue<std::string> cmdQueue;

    void waitInputLoop() {
        while (true) {
            std::string input;
            if (stopInput)
                break;
            std::getline(std::cin, input);
            cmdQueue.push(input);
        }
    }

    void execute(const std::string& cmd) {
        cmdQueue.push(cmd);
    }

    void startup() {
        std::thread inputThread(&waitInputLoop);
        inputThread.detach();
    }

    void newFile(const gfn::Uuid& documentUuid) {
        auto document = new gfn::document::Document(documentUuid, &preferences);
        documents.insert(document);
        document->startup();
    }

    void saveFile(gfn::document::Document* document) {
        waitingSerializedDoc = document;
        // request for a save by marking buffer out of date
        //waitingSerializedDoc->interface.serialized.readDone();
        // wait for response ///TODO
        serializeToken = gfn::uuid::createUuid();
        waitingSerialized = true;

        execute("save -token=" + serializeToken);
        //while (document->interface.serialized.isReadBufferRead()) {}
        //std::cout << gfn::editor::focusedDocument->interface.serialized.getRead()->dump(4, ' ', false) << "\n";
    }

    void update() {
        onFocusDocument = false;
        for (auto& d : documents) {
            if (d->isWindowFocused && focusedDocument != d) {
                focusedDocument = d;
                onFocusDocument = true;
            }
        }

        if (waitingSerialized) {
            if ((*waitingSerializedDoc->interface.serialized.getRead())["token"] == serializeToken) {
                // new data in
                std::cout << waitingSerializedDoc->interface.serialized.getRead()->dump(4, ' ', false) << "\n";
                waitingSerialized = false;
                waitingSerializedDoc = nullptr;
            }
            if (waitingSerializedDoc)
                waitingSerializedDoc->interface.serialized.readDone();
        }

        while (!cmdQueue.empty()) {
            std::string cmd = cmdQueue.front();
            gfn::Command command(cmd);
            cmdQueue.pop();

            if (command.getParamValue("command") == "cd") {
                if (!command.getParamValue("-d").empty()) {
                    for (auto& d : documents) {
                        if (d->documentUuid == command.getParamValue("-d"))
                            focusedDocument = d;
                    }
                } else
                    std::cerr << "Expected document uuid, use [-d]\n";
            } else {
                if (focusedDocument)
                    focusedDocument->execute(cmd);
                else
                    std::cerr << "No document focused\n";
            }
        }
        if (focusedDocument)
            focusedDocument->isWindowFocused = true;
        for (auto d = documents.begin(); d != documents.end();) {
            if (!(*d)->update()) {
                if (focusedDocument == *d)
                    focusedDocument = nullptr;
                (*d)->terminate();
                d = documents.erase(d);
            } else
                d++;
        }
    }

    void terminate() {
        for (auto& d : documents)
            d->terminate();
    }

}
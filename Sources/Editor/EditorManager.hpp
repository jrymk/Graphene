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

    bool stopInput = false;

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

    void update() {
        for (auto& d : documents) {
            if (d->isWindowFocused)
                focusedDocument = d;
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
            if (focusedDocument)
                std::cout << focusedDocument->documentUuid << "> ";
        }

        for (auto& d : documents)
            d->update();
    }

    void terminate() {
        for (auto& d : documents)
            d->terminate();
    }

}
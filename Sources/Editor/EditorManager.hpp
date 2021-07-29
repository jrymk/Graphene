#pragma once

#include <unordered_set>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include <Document/Document.hpp>

namespace gfn::editor {
    // one single preference object for the application
    gfn::preferences::Preferences preferences;

    std::unordered_set<gfn::document::Document*> documents;
    gfn::document::Document* focusedDocument;

    bool stopInput = false;

    std::stringstream ss;

    void waitInputLoop() {
        while (true) {
            std::string input;
            if (stopInput)
                break;
            std::getline(std::cin, input);
            ss << input;
        }
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
        if (!ss.str().empty()) {
            std::string document;
            ss >> document;
            for (auto& d : documents) {
                if (d->documentUuid == document) {
                    std::string cmd;
                    std::getline(ss, cmd);
                    d->execute(cmd);
                }
            }
            ss.str("");
        }

        for (auto& d : documents) {
            if (d->isWindowFocused)
                focusedDocument = d;
        }
        for (auto& d : documents)
            d->update();
    }

    void terminate() {
        for (auto& d : documents)
            d->terminate();
    }

}
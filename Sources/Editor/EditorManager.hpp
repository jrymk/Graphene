#pragma once

#include <unordered_set>
#include <Document/Document.hpp>

namespace gfn::editor {
    // one single preference object for the application
    gfn::preferences::Preferences preferences;

    std::unordered_set<gfn::document::Document*> documents;
    gfn::document::Document* focusedDocument;



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

        for (auto& d : documents)
            d->update();
    }

    void terminate() {
        for (auto& d : documents)
            d->terminate();
    }

}
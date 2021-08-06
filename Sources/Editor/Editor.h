#ifndef EDITOR_H
#define EDITOR_H

#include <queue>
#include <Editor/Document/Document.h>
#include <Editor/HotKeyHandler.h>
#include <Editor/Preferences/Preferences.h>
#include <Editor/Graphics/Graphics.h>

/// Editor is the entire GUI application, with document system, handles application preferences, documents,
/// graph view (real-time graph rendering) and more
namespace gfn {
    class Editor {
        gfn::Uuid activeDoc;
        gfn::HKHandler hk;
        // one single preference object for the entire application
        gfn::Preferences prefs;

        void updateDocuments();

    public:
        gfn::Graphics gfx; // TODO
        std::unordered_map<gfn::Uuid, gfn::Document*> documents;
        Editor();

        void update();

        gfn::Document* getDoc(const gfn::Uuid& docId);

        gfn::Uuid newDocument();

        void closeDocument(const gfn::Uuid& docId);

        void loadDocsFromArgs(int argc, char* argv[], bool doNotCreateUntitled);

        // stops all core instances
        void terminate();

        // deallocate memory
        //void cleanup();
    };
}

#endif
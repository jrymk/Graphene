#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <Core/Core/Core.h>
#include <Editor/Document/GraphView/GraphView.h>
#include <Editor/Graphics/Graphics.h>
#include <Core/Objects/Uuid.h>

namespace gfn {
    // documents are handled by gfn
    class Document {
    private:
        gfn::Core core;                        // updates vertex positions
        gfn::Interface* itf;         // to interact with the multithreaded core
        gfn::HKHandler* hk;                 // get hot key states from preferences.bindings
        gfn::Preferences* prefs;  // application preferences

        std::atomic<bool> _enableCoreUpdate = true;
        std::atomic<bool> _terminateCoreUpdate = false;

        static int untitledCounter;

    public:
        gfn::GraphView graphview; // handles graph rendering and interaction

        // obtain preferences from Editor
        Document(const gfn::Uuid& _docId, gfn::HKHandler* hk, gfn::Preferences* prefs, gfn::Graphics* gfx);

        void setFile(const std::string& _file, bool doNotSetDocName = false);

        // to run commands to the document core
        void execute(const std::string& cmd);

        void update();

        /// DOCUMENT PROPERTIES
        gfn::Uuid docId; // a unique identifer to access the document
        std::string docName; // name on the document tab
        std::string file; // the absolute path for the gfn file

        std::string getFileName() const;
        /// DOCUMENT PROPERTIES

        gfn::Interface* getItf();

        /// WINDOW STATE
        bool isFocused = false; // updated each frame
        bool fileSaved = false;
        bool showCloseConfirmationDialog = false;
        bool wantSaveAsAndClose = false;
        bool isDocumentWindowOpen = true; // the p_open for the document main window
        bool closeDocument = false; // the master switch to the document, one frame true and POOF
        bool isGraphUpdate = false;
        bool isGraphUpdateEx = false;
        bool isGraphStreaming = true;
        bool showExportPopup = false;
        /// WINDOW STATE

        /// GRAPH
        bool doZoomToFit = false;
        /// GRAPH

        void exportMenu();

        void exportTikZ();

        void exportSVG();

        void exportPNG();

        bool showTikZMenu = false;
        bool showSVGMenu = false;
        bool showPNGMenu = false;

        std::string _tikzColor(uint32_t color);

        void terminate();
    };
}

#endif
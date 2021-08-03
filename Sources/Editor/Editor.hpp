#pragma once

#include <Editor/Documents.hpp>
#include <Editor/Commands.hpp>
#include <Editor/PropertiesPanel.hpp>

/// Editor is the entire GUI application, with document system. Editor is in global scope under gfn::editor
// namespace, handles application preferences, documents, graph view (real-time graph rendering) and more

namespace gfn::editor {
    // one single preference object for the application
    gfn::preferences::Preferences preferences;

    void update() {
        updateDocManagement();
        for (auto &d : documents)
            d.second->update();
        updateFileDialog();

        parseCommandQueue();
    }

}
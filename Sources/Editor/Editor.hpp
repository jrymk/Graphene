#pragma once

#include <Editor/Documents.hpp>
#include <Editor/Commands.hpp>
#include <Editor/PropertiesPanel/PropertiesPanel.hpp>
#include <Preferences/KeyBind/KeyBind.hpp>

/// Editor is the entire GUI application, with document system. Editor is in global scope under gfn::editor
// namespace, handles application preferences, documents, graph view (real-time graph rendering) and more

namespace gfn::editor {
    // one single preference object for the entire application
    gfn::preferences::Preferences preferences;

    void init() {
        cmdStartup();
        initHotKey();
        preferences.keyBind = &keyBind;
        preferences.loadFromFile();
    }

    void updateMainWindow() {
        updateHotKey();
        updateDocManagement();
        updateFileDialog();

        parseCommandQueue();

        if (keyBind.keyBindUpdated) {
            preferences.saveToFile();
            keyBind.keyBindUpdated = false;
        }
    }

    void updateDocuments() {
        for (auto& d : documents)
            d.second->update();
    }
}
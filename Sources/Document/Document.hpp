#pragma once

#include <thread>
#include <atomic>
#include <Document/GraphView/GraphView.hpp>
#include <Preferences/Preferences.hpp>
#include <Objects/Uuid.hpp>

namespace gfn::document {
    class Document {
        gfn::core::Core core; // updates vertex positions
        gfn::interface::Interface interface; // to interact with the multithreaded core
        gfn::editor::graphview::GraphView graphview; // handles graph rendering and interaction
        gfn::preferences::Preferences* preferences; // application preferences

        std::atomic<bool> _enableCoreUpdate = true;
        std::atomic<bool> _terminateCoreUpdate = false;

    public:
        gfn::Uuid documentUuid;
        std::string savePath;
        bool isWindowFocused = false;

        explicit Document(const gfn::Uuid& _documentUuid, gfn::preferences::Preferences* _preferences) {
            documentUuid = _documentUuid;
            preferences = _preferences;
        };

        void startup() {
            core.bindInterface(&interface);
            graphview.init(documentUuid, &interface, preferences);

            // start up multithreaded core update
            core.bindInterface(&interface);
            std::thread coreThread(&Document::coreUpdate, this);
            coreThread.detach();
        }

        // endless core update loop
        void coreUpdate() {
            while (!_terminateCoreUpdate) {
                if (_enableCoreUpdate)
                    core.coreCycle();
            }
        }

        void execute(const std::string& cmd) {
            interface.cmdBuffer.getWrite()->commands.emplace_back(cmd);
        }

        // called every gui cycle
        void update() {
            graphview.update();

            isWindowFocused = graphview.isWindowFocused;
            interface.userprops.readDone();
            interface.configs.writeDone();
            interface.cmdBuffer.writeDone();

            //interface.cmdBuffer.getWrite()->commands.clear();
        }

        void terminate() { _terminateCoreUpdate = true; }
    };
} // namespace gfn::document
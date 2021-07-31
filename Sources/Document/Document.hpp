#pragma once

#include <thread>
#include <atomic>
#include <Document/GraphView/GraphView.hpp>
#include <Preferences/Preferences.hpp>
#include <Objects/Uuid.hpp>
#include <Document/Serialization/SaveFile.hpp>

namespace gfn::document {
    class Document {
    public:
        gfn::core::Core core;                         // updates vertex positions
        gfn::interface::Interface interface;         // to interact with the multithreaded core
        gfn::editor::graphview::GraphView graphview; // handles graph rendering and interaction
        gfn::preferences::Preferences* preferences;     // application preferences

        std::atomic<bool> _enableCoreUpdate = true;
        std::atomic<bool> _terminateCoreUpdate = false;

        gfn::Uuid documentUuid;
        std::string savePath;
        bool isWindowFocused = false;
        bool keepDocumentOpen = true;

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

        void execute(const std::string& cmd) { interface.cmdBuffer.getWrite().commands.emplace_back(cmd); }

        // called every gui cycle
        bool update() {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin(("Graph View #" + documentUuid).c_str(), &keepDocumentOpen, 0);

            graphview.update();

            if (isWindowFocused) {
                if (!graphview.selection.leftMouseUpVertex.empty()) {
                    execute("mkedge -u=" + graphview.selection.leftMouseDownVertex + " -v=" +
                            graphview.selection.leftMouseUpVertex);
                }
                if (!graphview.selection.hoveredVertex.empty()) {
                    if (ImGui::IsKeyPressed('D', false))
                        execute("rmvertex -uuid=" + graphview.selection.hoveredVertex);
                } else if (!graphview.selection.hoveredEdge.empty()) {
                    if (ImGui::IsKeyPressed('D', false))
                        execute("rmedge -uuid=" + graphview.selection.hoveredEdge);
                }
                if (ImGui::IsKeyPressed('V', false)) {
                    if (graphview.selection.hoveredVertex.empty() && !graphview.selection.hoveredEdge.empty()) {
                        auto eProp = interface.properties.getRead()->getEdgeProps(graphview.selection.hoveredEdge);
                        execute("rmedge -u=" + eProp->startVertexUuid.value + " -v=" + eProp->endVertexUuid.value);
                        auto vId = gfn::uuid::createUuid();
                        execute("mkvertex -uuid=" + vId);
                        execute("setvertexprop -uuid=" + vId +
                                " -key=position -value=(" + std::to_string(graphview.selection.cursorCoord.x)
                                + "," + std::to_string(graphview.selection.cursorCoord.y));
                        execute("mkedge -u=" + vId + " -v=" + eProp->startVertexUuid.value);
                        execute("mkedge -u=" + vId + " -v=" + eProp->endVertexUuid.value);
                    } else if (graphview.selection.hoveredVertex.empty()) {
                        auto vId = gfn::uuid::createUuid();
                        execute("mkvertex -uuid=" + vId);
                        execute("setvertexprop -uuid=" + vId +
                                " -key=position -value=(" + std::to_string(graphview.selection.cursorCoord.x)
                                + "," + std::to_string(graphview.selection.cursorCoord.y) + ")");
                    }
                }
            }

            interface.usergraph.readDone();
            interface.properties.readDone();
            interface.configs.writeDone();
            if (interface.cmdBuffer.wantWrite())
                interface.cmdBuffer.writeDone();

            isWindowFocused = ImGui::IsWindowFocused();

            // interface.cmdBuffer.getWrite()->commands.clear();
            ImGui::End();

            return keepDocumentOpen;
        }

        void terminate() { _terminateCoreUpdate = true; }
    };
} // namespace gfn::document
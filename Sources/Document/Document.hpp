#pragma once

#include <thread>
#include <atomic>
#include <Document/GraphView/GraphView.hpp>
#include <Preferences/Preferences.hpp>
#include <Objects/Uuid.hpp>

namespace gfn::document {
    class Document {
    public:
        gfn::core::Core core;                        // updates vertex positions
        gfn::interface::Interface interface;         // to interact with the multithreaded core
        gfn::editor::graphview::GraphView graphview; // handles graph rendering and interaction
        gfn::preferences::Preferences* preferences;  // application preferences

        std::atomic<bool> _enableCoreUpdate = true;
        std::atomic<bool> _terminateCoreUpdate = false;

        /// DOCUMENT PROPERTIES
        gfn::Uuid docId;
        std::string filePath;
        std::string displayName;

        std::string getFileName() const { return filePath.substr(filePath.find_last_of('/') + 1); }

        Document(const std::string& _displayName, const gfn::Uuid& _docId,
                 gfn::preferences::Preferences* _preferences) :
                core(&interface) {
            displayName = _displayName;
            docId = _docId;
            preferences = _preferences;
            graphview.init(docId, &interface, preferences);
            // start up multithreaded core update
            std::thread coreThread(&Document::coreUpdate, this);
            coreThread.detach();
        };
        /// DOCUMENT PROPERTIES

        /// WINDOW STATE
        bool isFocused = false;             // updated each frame. will be maintained by editormanager
        bool keepDocumentOpen = true;

        bool fileSaved = false;
        bool showCloseConfirmationDialog = false;
        bool isDocumentWindowOpen = true;         // the p_open for the document main window
        bool closeDocument = false;               // the master switch to the document, one frame true and POOF
        /// WINDOW STATE

        /// CORE
        void coreUpdate() {
            while (!_terminateCoreUpdate) {
                if (_enableCoreUpdate)
                    core.coreCycle();
            }
        }

        void execute(const std::string& cmd) { interface.cmdBuffer.getWrite().commands.emplace_back(cmd); }

        void terminate() { _terminateCoreUpdate = true; }
        /// CORE


        void update() {
            if (!closeDocument) {
                ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
                ImGui::Begin((displayName + "###" + docId).c_str(), &isDocumentWindowOpen,  0);
                graphview.update();

                isFocused = ImGui::IsWindowFocused();

                if (isFocused) {
                    ImGui::SetCursorPos(ImVec2(200, 200));
                    ImGui::Text("Focused");
                }
                if (isFocused) {
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
                            execute("setvertexprops -uuid=" + vId +
                                    " -key=position -value=(" + std::to_string(graphview.selection.cursorCoord.x)
                                    + "," + std::to_string(graphview.selection.cursorCoord.y));
                            execute("mkedge -u=" + vId + " -v=" + eProp->startVertexUuid.value);
                            execute("mkedge -u=" + vId + " -v=" + eProp->endVertexUuid.value);
                        } else if (graphview.selection.hoveredVertex.empty()) {
                            auto vId = gfn::uuid::createUuid();
                            execute("mkvertex -uuid=" + vId);
                            execute("setvertexprops -uuid=" + vId +
                                    " -key=position -value=\"(" + std::to_string(graphview.selection.cursorCoord.x)
                                    + ", " + std::to_string(graphview.selection.cursorCoord.y) + "\")");
                        }
                    }

                    static gfn::Vec2f downPos;
                    static gfn::Uuid mouseDownUuid;
                    if (!graphview.selection.middleMouseDownVertex.empty()) {
                        if (ImGui::GetIO().MouseClicked[ImGuiMouseButton_Middle]) {
                            downPos = graphview.selection.cursorCoord;
                            mouseDownUuid = graphview.selection.middleMouseDownVertex;
                            execute("setvertexprops -uuid=" + mouseDownUuid + " -key=pauseUpdate -value=true");
                        }
                        execute("setvertexprops -uuid=" + mouseDownUuid + " -key=position -value=\"(" +
                                std::to_string(downPos.x + graphview.selection.middleMousePosDelta.x)
                                + ", " + std::to_string(downPos.y + graphview.selection.middleMousePosDelta.y) + "\")");
                        if (ImGui::GetIO().MouseReleased[ImGuiMouseButton_Middle]) {
                            execute("setvertexprops -uuid=" + mouseDownUuid + " -key=pauseUpdate -value=false");
                        }
                    }
                }

                interface.usergraph.readDone();
                interface.properties.readDone();
                interface.configs.writeDone();
                if (interface.cmdBuffer.wantWrite())
                    interface.cmdBuffer.writeDone();

                // interface.cmdBuffer.getWrite()->commands.clear();
                ImGui::End();

                if (!isDocumentWindowOpen && !fileSaved)
                    showCloseConfirmationDialog = true;

                if (showCloseConfirmationDialog) {
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y
                                                                                        * 0.5f), ImGuiCond_Appearing,
                                            ImVec2(0.5f, 0.5f));
                    ImGui::SetNextWindowSize(ImVec2(360, 150), ImGuiCond_Appearing);

                    ImGui::OpenPopup("Confirmation");

                    ImGui::BeginPopupModal("Confirmation");
                    ImGui::Text(("Do you want to save changes to " + displayName + "?").c_str());
                    if (ImGui::Button("Save")) {
                        execute("save -f=\"" + filePath + "\"");
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Don't save")) {
                        ImGui::CloseCurrentPopup();
                        closeDocument = true;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        ImGui::CloseCurrentPopup();
                        showCloseConfirmationDialog = false;
                    }
                    ImGui::EndPopup();
                }
            }
        }

    };
} // namespace gfn::document
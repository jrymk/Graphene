#pragma once

#include <Core/Core.hpp>
#include <thread>
#include <atomic>
#include <Document/GraphView/GraphView.hpp>
#include <Preferences/Preferences.hpp>
#include <Objects/Uuid.h>

namespace gfn::document {
    class Document {
    public:
        gfn::core::Core core;                        // updates vertex positions
        gfn::Interface* itf;         // to interact with the multithreaded core
        gfn::graphview::GraphView graphview; // handles graph rendering and interaction
        gfn::preferences::Preferences* preferences;  // application preferences

        std::atomic<bool> _enableCoreUpdate = true;
        std::atomic<bool> _terminateCoreUpdate = false;

        /// DOCUMENT PROPERTIES
        gfn::Uuid docId;
        std::string filePath;
        std::string displayName;

        std::string getFileName() const { return filePath.substr(filePath.find_last_of('/') + 1); }

        Document(const std::string& _displayName, const gfn::Uuid& _docId,
                 gfn::preferences::Preferences* _preferences) {
            displayName = _displayName;
            docId = _docId;
            preferences = _preferences;
            itf = core.getInterface();
            graphview.init(docId, itf, preferences);
            // start up multithreaded core update
            std::thread coreThread(&Document::coreUpdate, this);
            coreThread.detach();
        };
        /// DOCUMENT PROPERTIES

        /// WINDOW STATE
        bool isFocused = false;             // updated each frame
        bool keepDocumentOpen = true;

        bool fileSaved = false;
        bool showCloseConfirmationDialog = false;
        bool wantSaveAsAndClose = false;
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

        void execute(const std::string& cmd) { itf->cmdBuf.getWrite().commands.emplace_back(cmd); }

        void terminate() { _terminateCoreUpdate = true; }
        /// CORE


        void update() {
            if (!closeDocument) {
                ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
                ImGui::Begin((displayName + "###" + docId).c_str(), &isDocumentWindowOpen, 0);

                graphview.update();

                isFocused = ImGui::IsWindowFocused();

                if (ImGui::IsWindowFocused()) {
                    if (graphview.selection.vertexSelection.empty() &&
                        !graphview.selection.mouseClickVertex[ImGuiMouseButton_Left].empty() &&
                        !graphview.selection.mouseOnReleaseVertex[ImGuiMouseButton_Left].empty()) {
                        execute("mkedge -u=" + graphview.selection.mouseClickVertex[ImGuiMouseButton_Left] + " -v=" +
                                graphview.selection.mouseOnReleaseVertex[ImGuiMouseButton_Left]);
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
                            auto eProp = itf->props.getRead()->getEdgeProps(graphview.selection.hoveredEdge);
                            execute("rmedge -u=" + eProp->startVertexUuid.value + " -v=" + eProp->endVertexUuid.value);
                            auto vId = gfn::createUuid();
                            execute("mkvertex -uuid=" + vId);
                            execute("setvertexprops -uuid=" + vId +
                                    " -key=position -value=(" + std::to_string(graphview.selection.mouseCoord.x)
                                    + "," + std::to_string(graphview.selection.mouseCoord.y));
                            execute("mkedge -u=" + vId + " -v=" + eProp->startVertexUuid.value);
                            execute("mkedge -u=" + vId + " -v=" + eProp->endVertexUuid.value);
                        } else if (graphview.selection.hoveredVertex.empty()) {
                            auto vId = gfn::createUuid();
                            execute("mkvertex -uuid=" + vId);
                            execute("setvertexprops -uuid=" + vId +
                                    " -key=position -value=\"(" + std::to_string(graphview.selection.mouseCoord.x)
                                    + ", " + std::to_string(graphview.selection.mouseCoord.y) + "\")");
                        }
                    }

                    if (graphview.selection.moveStarted) {
                        for (auto& v : graphview.selection.vertexSelection)
                            execute("setvertexprops -uuid=" + v + " -key=pauseUpdate -value=true");
                    }
                    if (graphview.selection.moving) {
                        for (auto& v : graphview.selection.vertexSelection) {
                            execute("setvertexprops -uuid=" + v + " -key=position -value=\"+(" +
                                    std::to_string(graphview.selection.mouseDelta.x)
                                    + ", " + std::to_string(graphview.selection.mouseDelta.y) +
                                    "\")");
                        }
                    }
                    if (graphview.selection.moveEnded) {
                        for (auto& v : graphview.selection.vertexSelection)
                            execute("setvertexprops -uuid=" + v + " -key=pauseUpdate -value=false");
                    }
                }

                itf->graph.readDone();
                itf->props.readDone();
                itf->cfg.writeDone();
                if (itf->cmdBuf.wantWrite())
                    itf->cmdBuf.writeDone();

                // interface.cmdBuf.getWrite()->commands.clear();
                ImGui::End();

                if (!isDocumentWindowOpen) {
                    if (fileSaved)
                        closeDocument = true;
                    else
                        showCloseConfirmationDialog = true;
                }

                if (showCloseConfirmationDialog) {
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y
                                                                                        * 0.5f), ImGuiCond_Appearing,
                                            ImVec2(0.5f, 0.5f));
                    ImGui::SetNextWindowSize(ImVec2(360, 150), ImGuiCond_Appearing);

                    ImGui::OpenPopup(("Close confirmation"));

                    ImGui::BeginPopupModal(("Close confirmation"));
                    ImGui::Text(("Do you want to save changes to " + displayName + "?").c_str());
                    if (ImGui::Button("Save")) {
                        if (filePath.empty())
                            wantSaveAsAndClose = true;
                        else {
                            execute("save -f=\"" + filePath + "\"");
                            closeDocument = true;
                        }
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Don't save")) {
                        closeDocument = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        showCloseConfirmationDialog = false;
                        isDocumentWindowOpen = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
                }
            }
        }
    };
} // namespace gfn::document
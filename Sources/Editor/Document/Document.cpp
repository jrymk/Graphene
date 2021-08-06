#include "Document.h"

namespace gfn {
    int Document::untitledCounter = 1;


    Document::Document(gfn::Uuid docId, gfn::HKHandler* hk, gfn::Preferences* prefs) :
            core(),
            itf(core.getInterface()),
            docId(std::move(docId)),
            hk(hk),
            prefs(prefs),
            graphview(docId, itf, hk, prefs) {
        graphview.hk = hk;
        docName = "Untitled (" + std::to_string(untitledCounter) + ")";
        untitledCounter++;
        // start core background update loop
        core.startBackground();
    };

    void Document::setFile(const std::string& _file, bool doNotSetDocName) {
        file = _file;
        std::replace(file.begin(), file.end(), '\\', '/'); // windows is nasty
        core.file = _file;
        docName = getFileName();
        untitledCounter--;
    }

    void Document::execute(const std::string& cmd) { itf->commands.getWrite().buffer.emplace_back(cmd); }

    void Document::update() {
        if (!closeDocument) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin((docName + "###" + docId).c_str(), &isDocumentWindowOpen, ImGuiWindowFlags_UnsavedDocument);

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
                        auto eProp = itf->graph.getRead()->props.getEdgeProps(graphview.selection.hoveredEdge);
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
            if (itf->commands.wantWrite())
                itf->commands.writeDone();

            // interface.commands.getWrite()->commands.clear();
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
                ImGui::Text(("Do you want to save changes to " + docName + "?").c_str());
                if (ImGui::Button("Save")) {
                    if (file.empty())
                        wantSaveAsAndClose = true;
                    else {
                        execute("save -f=\"" + file + "\"");
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

    void Document::terminate() {
        core.terminateBackground();
    }

    std::string Document::getFileName() const { return file.substr(file.find_last_of('/') + 1); }

    Interface* Document::getItf() {
        return itf;
    }
}

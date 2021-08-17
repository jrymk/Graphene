#include "Document.h"
#include <Tracy.hpp>

namespace gfn {
    int Document::untitledCounter = 1;


    Document::Document(const gfn::Uuid& _docId, gfn::HKHandler* hk, gfn::Preferences* prefs, gfn::Graphics* gfx) :
            core(),
            itf(core.getInterface()),
            docId(_docId),
            hk(hk),
            prefs(prefs),
            graphview(_docId, itf, hk, prefs, gfx) {
        graphview.hk = hk;
        if (_docId == gfn::createNil()) {
            docName = "\ue86f Terminal";
        } else {
            docName = "\ue24d Untitled (" + std::to_string(untitledCounter) + ")";
            untitledCounter++;
            execute("echo -off");
        }
        // start core background update loop
        core.startBackground();
    }

    void Document::setFile(const std::string& _file, bool doNotSetDocName) {
        file = _file;
        std::replace(file.begin(), file.end(), '\\', '/'); // windows is nasty
        core.file = _file;
        docName = "\uea0d " + getFileName();
        untitledCounter--;
    }

    void Document::execute(const std::string& cmd) {
        if (itf->graph.getRead()->cfg.energySavingMode)
            itf->commands.getWrite().buffer.emplace_back("configs -resume");
        itf->commands.getWrite().buffer.emplace_back(cmd);
    }

    void Document::update() {
        ZoneScoped

        if (!closeDocument) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin((docName + "###" + docId).c_str(), &isDocumentWindowOpen, ImGuiWindowFlags_UnsavedDocument);

            if (doZoomToFit) {
                graphview.camera.zoom = (1.0 / std::max(graphview.camera.xMax - graphview.camera.xMin, graphview.camera.yMax - graphview.camera.yMin)) *
                                        std::min(ImGui::GetWindowWidth(), ImGui::GetWindowHeight()) * 0.8;
                graphview.camera.centerCoord.x = (graphview.camera.xMax + graphview.camera.xMin) / 2.0;
                graphview.camera.centerCoord.y = (graphview.camera.yMax + graphview.camera.yMin) / 2.0;
                doZoomToFit = false;
            }

            graphview.update();

            while (!graphview.commands.empty()) {
                execute(graphview.commands.front());
                graphview.commands.pop();
            }

            isFocused = ImGui::IsWindowFocused();


            if (ImGui::IsWindowFocused()) {
                if ((hk->press(TOGGLE_GRAPH_UPDATE) || hk->press(PAUSE_GRAPH_UPDATE)) && isGraphUpdate) {
                    isGraphUpdate = false;
                    isGraphUpdateEx = true;
                } else if ((hk->press(TOGGLE_GRAPH_UPDATE) || hk->release(PAUSE_GRAPH_UPDATE)) && !isGraphUpdate) {
                    isGraphUpdate = true;
                    isGraphUpdateEx = true;
                }
                if ((hk->press(TOGGLE_GRAPH_STREAMING) || hk->press(PAUSE_GRAPH_STREAMING)) && isGraphStreaming)
                    isGraphStreaming = false;
                else if ((hk->press(TOGGLE_GRAPH_STREAMING) || hk->release(PAUSE_GRAPH_STREAMING)) && !isGraphStreaming)
                    isGraphStreaming = true;
            }

            if (isGraphUpdateEx) {
                if (!isGraphUpdate)
                    execute("configs -pause");
                else
                    execute("configs -resume");
//                    core.startBackground();
//                    core.terminateBackground();
                isGraphUpdateEx = false;
            }
            if (isGraphStreaming)
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

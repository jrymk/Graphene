#include "../Document.h"
#include <Editor/Theme/Theme.h>

namespace gfn {
    void Document::exportMenu() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(std::max(300.0f, 100.0f), std::max(200.0f, 100.0f)), ImGuiCond_Appearing);

        ImGui::BeginPopupModal("\ue6b8 Export");

        if (gfn::button("Export as TikZ picture", HUE_CONTRAST, HUE_CONTRAST, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            ImGui::OpenPopup("TikZ picture Export");
            showTikZMenu = true;
        }
        if (showTikZMenu) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(250.0f, 100.0f), std::max(250.0f, 100.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("TikZ picture Export");
            exportTikZ();
            ImGui::EndPopup();
        }

        if (gfn::button("Export as SVG", HUE_CONTRAST, HUE_CONTRAST, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            ImGui::OpenPopup("SVG Export");
            showSVGMenu = true;
        }
        if (showSVGMenu) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(250.0f, 100.0f), std::max(250.0f, 100.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("SVG Export");
            exportSVG();
            ImGui::EndPopup();
        }

        if (gfn::button("Export as PNG", HUE_CONTRAST, HUE_CONTRAST, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            ImGui::OpenPopup("PNG Export");
            showPNGMenu = true;
        }
        if (showPNGMenu) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(150.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(250.0f, 100.0f), std::max(250.0f, 100.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("PNG Export");
            exportPNG();
            ImGui::EndPopup();
        }

        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 32.0f);
        if (gfn::button("\ue876 Done", HUE_GREEN, HUE_DEFAULT, false, ImGui::GetContentRegionAvailWidth(), 0, false)) {
            showExportPopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
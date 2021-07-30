#pragma once

#include <imgui.h>
#include <Interface/Interface.hpp>
#include <Editor/GraphView/Camera.hpp>
#include <Editor/GraphView/Camera.hpp>
#include <Preferences/Preferences.hpp>

namespace gfn::editor::graphview {
    ///@brief renders and handles interaction to a specified interface
    class GraphView {
        gfn::Uuid documentUuid;
        gfn::interface::Interface* interface;
        gfn::preferences::Preferences* preferences;

        gfn::editor::graphview::Camera camera;

    public:
        GraphView() = default;

        void bindInterface(gfn::interface::Interface* _interface) { interface = _interface; }

        void bindPreferences(gfn::preferences::Preferences* _preferences) { preferences = _preferences; }

        void bindDocumentUuid(const gfn::Uuid& _documentUuid) { documentUuid = _documentUuid; }

        void drawEdges() {
            auto drawList = ImGui::GetWindowDrawList();
            auto userprops = interface->userprops.getRead();
            for (auto& ei : userprops->getEdgePropList()) {
                auto e = ei.second;
                drawList->AddLine(camera.map(userprops->getVertexProps(e.startVertexUuid)->position),
                                  camera.map(userprops->getVertexProps(e.endVertexUuid)->position),
                                  e.edgeFillColor.color32,
                                  camera.map(e.thickness));
            }
        }

        void update() {
            ImGui::SetNextWindowSizeConstraints(ImVec2(100.0f, 100.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin(("Graph View #" + documentUuid).c_str());
            //ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            //auto binding = preferences->getKeyBind().getBinding(gfn::keybind::ACTION_CAMERA_PAN);
            ImGui::InvisibleButton("graphview_canvas", ImGui::GetWindowSize(),
                                   ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

            camera.update(preferences);
            drawEdges();
            //ImGui::PopStyleVar(1);
            ImGui::End();
        }
    };
}
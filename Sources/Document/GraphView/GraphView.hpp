#pragma once

#include <imgui.h>
#include <Interface/Interface.hpp>
#include <Preferences/Preferences.hpp>

#include <Document/GraphView/Camera.hpp>
#include <Document/GraphView/Renderer.hpp>
#include <Document/GraphView/Selection.hpp>

namespace gfn::editor::graphview {
///@brief renders and handles interaction to a specified interface
    class GraphView {
    public:
        gfn::Uuid documentUuid;
        gfn::interface::Interface* interface = nullptr;
        gfn::preferences::Preferences* preferences = nullptr;

        Camera camera;
        Renderer renderer;
        Selection selection;

        bool isWindowFocused = false;

        GraphView() = default;

        void init(const gfn::Uuid& _documentUuid, gfn::interface::Interface* _interface,
                  gfn::preferences::Preferences* _preferences) {
            documentUuid = _documentUuid;
            interface = _interface;
            preferences = _preferences;

            renderer.interface = _interface;
            renderer.camera = &camera;

            selection.interface = _interface;
            selection.preferences = preferences;
            selection.camera = &camera;
        }

        void update() {
            assert(interface && preferences); // did you forget to bind interface or preferences?

            /*if (interface->userprops.getRead()->getEdgePropList().size() > 0) {
                gfn::timer::Timer timer;
                for (int i = 0; i < 10000; i++) {
                    auto list = interface->userprops.getRead()->getEdgePropList();
                    auto v = interface->userprops.getRead()->getVertexProps((list.begin()->second.startVertexUuid));
                }
                std::cout << "Find 10000 props took " << timer.getMicroseconds() << "us\n";
            }*/
            ImGui::SetNextWindowSizeConstraints(ImVec2(100.0f, 100.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::Begin(("Graph View #" + documentUuid).c_str());
            isWindowFocused = ImGui::IsWindowFocused();

            // ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
            // auto binding = preferences->getKeyBind().getBinding(gfn::keybind::ACTION_CAMERA_PAN);
            ImGui::InvisibleButton("graphview_canvas", ImVec2(std::max(ImGui::GetContentRegionAvail().x, 10.0f),
                                                              std::max(ImGui::GetContentRegionAvail().y, 10.0f)),
                                   ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

            camera.update(preferences);
            selection.update();

            renderer.drawEdges();
            renderer.drawVertices();

            if (selection.hoveredVertex != gfn::uuid::createNil()) {
                auto props = interface->userprops.getRead()->getVertexProps(selection.hoveredVertex);
                ImGui::GetWindowDrawList()->AddCircle(camera.map(props->position), camera.map(props->radius),
                                                      IM_COL32(0, 255, 0, 255), 0, 1.0f);
            }
            // ImGui::PopStyleVar(1);
            ImGui::End();
        }
    };
} // namespace gfn::editor::graphview
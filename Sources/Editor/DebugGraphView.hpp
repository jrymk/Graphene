#pragma once

#include <imgui.h>
#include <Properties/UserProps.hpp>

namespace gfn::editor::debuggraphview {
    void show(gfn::properties::UserProps* prop) {
        ImGui::Begin("Debug Graph View");

        //for (auto& e : prop->getEdgePropList()) {
        for (auto it = prop->getEdgePropList().begin(); it != prop->getEdgePropList().end(); it++) {
            if (prop->getVertexProps(it->second.startVertexUuid) && prop->getVertexProps(it->second.endVertexUuid)) {
                ImGui::GetWindowDrawList()->AddLine(
                        ImVec2(ImGui::GetCursorScreenPos().x +
                               prop->getVertexProps(it->second.startVertexUuid)->position.x * 50.0 +
                               400.0,
                               ImGui::GetCursorScreenPos().y +
                               prop->getVertexProps(it->second.startVertexUuid)->position.y * 50.0 +
                               400.0),
                        ImVec2(ImGui::GetCursorScreenPos().x +
                               prop->getVertexProps(it->second.endVertexUuid)->position.x * 50.0 +
                               400.0,
                               ImGui::GetCursorScreenPos().y +
                               prop->getVertexProps(it->second.endVertexUuid)->position.y * 50.0 +
                               400.0),
                        IM_COL32(0, 0, 0, 255), 3.0f);
            }
        }

        for (auto& v : prop->getVertexPropList()) {
            ImGui::GetWindowDrawList()->AddCircleFilled(
                    ImVec2(ImGui::GetCursorScreenPos().x + v.second.position.x * 50.0 + 400.0,
                           ImGui::GetCursorScreenPos().y + v.second.position.y * 50.0 + 400.0),
                    5.0, IM_COL32(0, 0, 0, 255), 0);
        }

        ImGui::End();
    }
} // namespace gfn::editor::debuggraphview
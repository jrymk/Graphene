#pragma once

#include <imgui.h>
#include <Core/Properties/Properties.hpp>

namespace gfn::editor::debuggraphview {
void show(gfn::core::properties::Properties* prop) {
	ImGui::Begin("Debug Graph View");

	for (auto& e : prop->getEdgePropList()) {
		ImGui::GetWindowDrawList()->AddLine(
			ImVec2(ImGui::GetCursorScreenPos().x + prop->getVertexProp(e.second.startVertexUuid)->position.x * 50.0 +
					   400.0,
				   ImGui::GetCursorScreenPos().y + prop->getVertexProp(e.second.startVertexUuid)->position.y * 50.0 +
					   400.0),
			ImVec2(ImGui::GetCursorScreenPos().x + prop->getVertexProp(e.second.endVertexUuid)->position.x * 50.0 +
					   400.0,
				   ImGui::GetCursorScreenPos().y + prop->getVertexProp(e.second.endVertexUuid)->position.y * 50.0 +
					   400.0),
			IM_COL32(0, 0, 0, 255), 3.0f);
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
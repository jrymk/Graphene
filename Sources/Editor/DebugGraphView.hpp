#pragma once

#include <imgui.h>
#include <Core/Properties/Properties.hpp>

namespace gfn::editor::debuggraphview {
void show(gfn::core::properties::Properties* prop) {
	ImGui::Begin("Debug Graph View");

	for (auto& v : prop->getVertexPropList()) {
		ImGui::GetWindowDrawList()->AddCircleFilled(
			ImVec2(ImGui::GetCursorScreenPos().x + v.second.position.x * 800.0 + 400.0,
				   ImGui::GetCursorScreenPos().y + v.second.position.y * 800.0 + 400.0),
			2.0, IM_COL32(0, 0, 0, 255), 0);
	}

	ImGui::End();
}
} // namespace gfn::editor::debuggraphview
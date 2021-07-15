#pragma once

#include "../../../../Dependencies/imgui/imgui.h"

#include "Common.hpp"

namespace graphene::editor::graphview::contextmenu {

/*void update() {
	Controls::contextMenuOpen = false;
	//if (Controls::forceOpenContextMenu) {
	//	Controls::forceOpenContextMenu = false;
	if (ImGui::BeginPopupContextItem()) {
		Controls::contextMenuOpen = true;
		if (Controls::hoveredVertex != nullptr) {
			ImGui::Text(("Vertex: " + Controls::hoveredVertex->uid).c_str());
			if (ImGui::Button("Delete")) {
				Graphene::core->getGraphObj()->deleteVertex(Controls::hoveredVertex);
				Controls::contextMenuOpen = false;
			}
			ImGui::InputText("Vertex name", Controls::hoveredVertex->name, 1024, 0, 0, 0);
			if (ImGui::IsItemFocused())
				Controls::forceOpenContextMenu = true;
		} else if (Controls::hoveredComponent != nullptr) {
			ImGui::Text(("Component: " + Controls::hoveredComponent->uid).c_str());
			if (ImGui::Button("Disassemble")) {
				Graphene::core->getGraphObj()->deleteConnectedComponent(Controls::hoveredComponent, false);
				Controls::contextMenuOpen = false;
			}
			if (ImGui::Button("Delete")) {
				Graphene::core->getGraphObj()->deleteConnectedComponent(Controls::hoveredComponent, true);
				Controls::contextMenuOpen = false;
			}
			ImGui::ColorPicker3("Color", &Controls::hoveredComponent->color.x, ImGuiColorEditFlags_HSV);
		} else
			ImGui::Text("No selected object");

		ImGui::EndPopup();
	}
	//}
}*/

} // namespace graphene::editor::graphview::contextmenu
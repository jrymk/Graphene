#pragma once

#include "../../../../Dependencies/imgui/imgui.h"
#include "View.hpp"
#include "Overlay.hpp"
#include "Renderer.hpp"
#include "ContextMenu.hpp"
#include "Controls.hpp"
#include "ConvexHull.hpp"

namespace graphene::editor::graphview {

void updateKeyboardShortcuts() {
	// keyboard shortcut to toggle auto adjust view
	// if (ImGui::IsKeyPressed('A', false))
	//	controls::enableAutoAdjustView = !controls::enableAutoAdjustView;
	// if (ImGui::IsKeyPressed('V', true))
	//	Graphene::core->getGraphObj()->newVertex();
}

void show(/*::graphene::core::Core* core*/) {
	// Graphene::core = core;

	ImGui::SetNextWindowSizeConstraints(ImVec2(300, 350), ImVec2(FLT_MAX, FLT_MAX));

	ImGui::Begin(u8"Graph View on rendering thread", 0, ImGuiWindowFlags_NoCollapse);

	/*ImGui::Checkbox("Enable render", &controls::enableRender);
	ImGui::SameLine();
	ImGui::Checkbox("Show grid", &controls::gridVisible);
	ImGui::SameLine();
	ImGui::Checkbox("Auto adjust view (A)", &controls::enableAutoAdjustView);*/

	/*if (!controls::enableAutoAdjustView) {
		ImGui::SameLine();
		if (ImGui::Button("Reset view")) {
			view::zoomTarget = 1.0f;
			view::centerContext = ImVec2(0.5f, 0.5f);
		}
	}*/

	view::canvasBegin();

	// Graphene::core->getGraphObj()->updateConnectedComponent(); // continuously running this command is not a good
	// idea

	if (controls::enableAutoAdjustView)
		view::autoAdjustView();
	if (!controls::contextMenuOpen)
		updateKeyboardShortcuts();

	/*if (!controls::contextMenuOpen) {
		controls::updateHoveredVertex();
		if (controls::leftMouseSelectionVertex != nullptr) {
			controls::leftMouseSelectionVertex->directMove(
				Vec2f(ImGui::GetIO().MouseDelta.x / view::canvasFrameSize / view::zoomLevel,
					  -ImGui::GetIO().MouseDelta.y / view::canvasFrameSize / view::zoomLevel));
			controls::leftMouseSelectionVertex->flushMove(0.0f);
		}
		controls::updateHoveredComponent();
	}
	controls::updateMouse();*/

	// if (controls::enableRender) {
	view::updateCamera();
	view::updateMouseCursor();

	if (controls::gridVisible)
		renderer::drawGrid();

	// renderer::drawComponents();
	renderer::drawEdges();
	renderer::drawVertices();
	//}
	/*else {
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(200, 100, 100, 255));
		std::string tempStr("Rendering is disabled");
		char* label = new char[tempStr.length() + 1];
		strcpy(label, tempStr.c_str());
		ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
		ImVec2 labelAlign(0.5f, 0.5f);
		const ImRect bb(view::canvasCursor,
						{view::canvasCursor.x + view::canvasSize.x, view::canvasCursor.y + view::canvasSize.y});

		ImGui::RenderTextClipped(bb.Min, bb.Max, label, 0, &labelSize, labelAlign, &bb);
		ImGui::PopStyleColor(1);
	}*/

	// ContextMenu::update();
	overlay::draw();

	view::canvasEnd();
}
} // namespace graphene::editor::graphview
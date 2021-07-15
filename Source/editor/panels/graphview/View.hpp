#pragma once

#include <algorithm>
#include "../../../../Dependencies/imgui/imgui.h"
#include "../../../core/Vertex.hpp"
#include "../../../core/Edge.hpp"
#include "Common.hpp"

namespace graphene::editor::graphview::view {

double mapToContext(double l) { return l / canvasFrameSize / zoomLevel; }

double mapToCanvas(double l) { return l * canvasFrameSize * zoomLevel; }

/*::Graphene::Vec2f mapToContext(ImVec2 c) {
	return {canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel,
			canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel};
}*/

ImVec2 mapToCanvas(double x, double y) {
	return {float(canvasOrigin.x - (centerContext.x - x) * canvasFrameSize * zoomLevel),
			float(canvasOrigin.y + (centerContext.y - y) * canvasFrameSize * zoomLevel)};
}

ImVec2 mapToCanvas(Vec2f c) {
	return {float(canvasOrigin.x - (centerContext.x - c.x) * canvasFrameSize * zoomLevel),
			float(canvasOrigin.y + (centerContext.y - c.y) * canvasFrameSize * zoomLevel)};
}

void canvasBegin() {
	// Sets the canvas display attributes
	canvasSize = ImGui::GetContentRegionAvail();
	canvasCursor = ImGui::GetCursorScreenPos();
	canvasOrigin = ImVec2(canvasCursor.x + canvasSize.x / 2.0f, canvasCursor.y + canvasSize.y / 2.0f);
	canvasMargin = ImVec2(100.0f, 100.0f);
	canvasFrameSize =
		std::max(std::min(canvasSize.x - canvasMargin.x * 2.0f, canvasSize.y - canvasMargin.y * 2.0f), FLT_MIN);

	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->PushClipRect(canvasCursor, {canvasCursor.x + canvasSize.x, canvasCursor.y + canvasSize.y}, true);
	// Draw border and background color
	drawList->AddRectFilled(canvasCursor, {canvasCursor.x + canvasSize.x, canvasCursor.y + canvasSize.y},
							IM_COL32(0, 0, 0, 255), 6.0f, 0);
	// Catch mouse interactions
	ImGui::InvisibleButton("canvas", canvasSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	isHovered = ImGui::IsItemHovered();
}

void canvasEnd() {
	framerateCounter.resetCycle();

	ImGui::GetWindowDrawList()->PopClipRect();
	ImGui::End();
}

void autoAdjustView() {
	double x_max = -1000000000.0;
	double x_min = 1000000000.0;
	double y_max = -1000000000.0;
	double y_min = 1000000000.0;

	/*::graphene::core::VertexIter it(Graphene::core->getGraphObj());
	while (it.next()) {
		x_max = std::max(x_max, it.v->getCoord().x);
		x_min = std::min(x_min, it.v->getCoord().x);
		y_max = std::max(y_max, it.v->getCoord().y);
		y_min = std::min(y_min, it.v->getCoord().y);
	}
*/
	// if (rightMouseDownVertex == nullptr) {
	if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
		view::centerContext.x += ((x_max - x_min) / 2.0f + x_min - view::centerContext.x) * 0.2;
		view::centerContext.y += ((y_max - y_min) / 2.0f + y_min - view::centerContext.y) * 0.2;

		view::zoomTarget += (1.0f / std::max(std::max(x_max - x_min, y_max - y_min), 0.1) - view::zoomLevel) * 0.2;
	}
}

void updateCamera() {
	if (isHovered) {
		view::zoomTarget *= powf(1.05, ImGui::GetIO().MouseWheel);

		/*if (controls::rightMouseDownVertex == nullptr) {
			if (!controls::contextMenuOpen && ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
				view::centerContext.x += -ImGui::GetIO().MouseDelta.x / view::canvasFrameSize / view::zoomLevel;
				view::centerContext.y -= -ImGui::GetIO().MouseDelta.y / view::canvasFrameSize / view::zoomLevel;
			}
		}*/
		view::centerContext.x +=
			-(ImGui::GetIO().MousePos.x - view::canvasOrigin.x +
			  (view::canvasOrigin.x - ImGui::GetIO().MousePos.x) * (powf(1.05, ImGui::GetIO().MouseWheel))) /
			view::canvasFrameSize / view::zoomLevel;
		view::centerContext.y +=
			(ImGui::GetIO().MousePos.y - view::canvasOrigin.y +
			 (view::canvasOrigin.y - ImGui::GetIO().MousePos.y) * (powf(1.05, ImGui::GetIO().MouseWheel))) /
			view::canvasFrameSize / view::zoomLevel;
	}

	// smoothy transition zoom level
	view::zoomLevel *= powf(view::zoomTarget / view::zoomLevel, 0.1);
	view::zoomLevel = std::max(view::zoomTarget, DBL_MIN);
}

void updateMouseCursor() {
	/*if (view::isHovered) {
		if (controls::hoveredVertex != nullptr || controls::leftMouseDownVertex != nullptr ||
			controls::rightMouseDownVertex != nullptr)
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		else
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}*/
}

} // namespace graphene::editor::graphview::view
#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "../utils/ProfilerUtils.hpp"

namespace Gui {
	namespace GraphView {

		Utils::FramerateCounter framerateCounter;

		ImVec2 drawSize;
		ImVec2 topLeftDrawCoord;
		ImVec2 centerDrawCoord;
		ImVec2 canvasMargin;
		float canvasDisplaySize;

		ImVec2 centerMapped(0.5f, 0.5f);
		float zoomLevel = 1.0f;
		float zoomTarget = 1.0f;

		bool enableAutoAdjustView = true;
		bool gridVisible = true;

		bool isHovered = false;

		Graphene::Vertex* leftMouseDownVertex = nullptr;
		Graphene::Vertex* rightMouseDownVertex = nullptr;
		Graphene::Vertex* hoveredVertex = nullptr;

		void canvasBegin() {
			drawSize = ImGui::GetContentRegionAvail();
			topLeftDrawCoord = ImGui::GetCursorScreenPos();
			centerDrawCoord = ImVec2(
					topLeftDrawCoord.x + drawSize.x / 2.0f,
					topLeftDrawCoord.y + drawSize.y / 2.0f
			);
			canvasMargin = ImVec2(100.0f, 100.0f);
			canvasDisplaySize = std::max(std::min(drawSize.x - canvasMargin.x * 2.0f, drawSize.y - canvasMargin.y * 2.0f), FLT_MIN);

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			drawList->PushClipRect(
					topLeftDrawCoord,
					{topLeftDrawCoord.x + drawSize.x, topLeftDrawCoord.y + drawSize.y},
					true
			);
			// Draw border and background color
			drawList->AddRectFilled(
					topLeftDrawCoord,
					{topLeftDrawCoord.x + drawSize.x, topLeftDrawCoord.y + drawSize.y},
					IM_COL32(0, 0, 0, 255), 6.0f, 0
			);
			// catch mouse interactions
			ImGui::InvisibleButton("canvas", drawSize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

			isHovered = ImGui::IsItemHovered();
		}

		void canvasEnd() {
			if (!enableAutoAdjustView)
				ImGui::GetWindowDrawList()->AddText(
						ImVec2(topLeftDrawCoord.x + 4.0f, topLeftDrawCoord.y + drawSize.y - 36.0f),
						IM_COL32(200, 200, 200, 200), "Pan: Hold right mouse button   Zoom: Scroll", 0
				);
			else
				ImGui::GetWindowDrawList()->AddText(
						ImVec2(topLeftDrawCoord.x + 4.0f, topLeftDrawCoord.y + drawSize.y - 36.0f),
						IM_COL32(200, 200, 200, 200), "Auto adjust view enabled, manual pan and zoom is disabled", 0
				);

			ImGui::GetWindowDrawList()->AddText(
					ImVec2(topLeftDrawCoord.x + 4.0f, topLeftDrawCoord.y + drawSize.y - 20.0f),
					IM_COL32(200, 200, 200, 200),
					"Drag from vertex to vertex with left mouse button to create/delete edge, drag vertex with right mouse button to nudge vertex position", 0);

			ImGui::End();
		}

		void autoAdjustView(Graphene::Graph* graph) {
			float x_max = -1000000000.0;
			float x_min = 1000000000.0;
			float y_max = -1000000000.0;
			float y_min = 1000000000.0;

			Graphene::VertexIter it(graph);
			while (it.next()) {
				x_max = std::max(x_max, it.v->getCoord().x);
				x_min = std::min(x_min, it.v->getCoord().x);
				y_max = std::max(y_max, it.v->getCoord().y);
				y_min = std::min(y_min, it.v->getCoord().y);
			}

			//if (rightMouseDownVertex == nullptr) {
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
				centerMapped.x += ((x_max - x_min) / 2.0f + x_min - centerMapped.x) * 0.2f;
				centerMapped.y += ((y_max - y_min) / 2.0f + y_min - centerMapped.y) * 0.2f;

				zoomTarget += (1.0f / std::max(std::max(x_max - x_min, y_max - y_min), 0.1f) - zoomLevel) * 0.2f;
			}
		}

		ImVec2 getDrawCoord(Graphene::Vec2f mappedCoord) {
			return {centerDrawCoord.x - (centerMapped.x - mappedCoord.x) * canvasDisplaySize * zoomLevel,
			        centerDrawCoord.y + (centerMapped.y - mappedCoord.y) * canvasDisplaySize * zoomLevel};
		}

		Graphene::Vec2f getMappedCoord(ImVec2 drawCoord) {
			return {centerDrawCoord.x - (centerMapped.x - drawCoord.x) * canvasDisplaySize * zoomLevel,
			        centerDrawCoord.y + (centerMapped.y - drawCoord.y) * canvasDisplaySize * zoomLevel};
		}

		void updateKeyboardShortcuts() {
			// keyboard shortcut to toggle auto adjust view
			if (ImGui::IsKeyPressed('A', false))
				enableAutoAdjustView = !enableAutoAdjustView;
		}

		void updateHoveredVertex(Graphene::Core* core, Graphene::Graph* graph) {
			hoveredVertex = nullptr;

			float closestVertexDistance = FLT_MAX;

			Graphene::VertexIter it(graph);

			while (it.next()) {
				float mouseVertexDistanceSquared =
						powf(ImGui::GetIO().MousePos.x - getDrawCoord(it.v->getCoord()).x, 2.0f) +
						powf(ImGui::GetIO().MousePos.y - getDrawCoord(it.v->getCoord()).y, 2.0f);

				if (ImGui::IsItemHovered()) {
					if (mouseVertexDistanceSquared <= powf(40.0f * powf(zoomTarget, 0.1), 2.0f)) {
						if (mouseVertexDistanceSquared <= closestVertexDistance) {
							hoveredVertex = it.v;
							closestVertexDistance = mouseVertexDistanceSquared;
						}
					}
				}
			}


			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && hoveredVertex != nullptr) {
				leftMouseDownVertex = hoveredVertex;
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				if (leftMouseDownVertex != nullptr && hoveredVertex != nullptr) {
					if (graph->isAdjacent(leftMouseDownVertex->getNumber(), hoveredVertex->getNumber()))
						graph->removeEdge(leftMouseDownVertex->getNumber(), hoveredVertex->getNumber());
					else if (graph->isAdjacent(hoveredVertex->getNumber(), leftMouseDownVertex->getNumber()))
						graph->removeEdge(hoveredVertex->getNumber(), leftMouseDownVertex->getNumber());
					else
						graph->addEdge(leftMouseDownVertex->getNumber(), hoveredVertex->getNumber());

					core->boundGraph()->evalConnectedComponent();

					core->pendingInputUpdate = true;
				}

				leftMouseDownVertex = nullptr;
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && hoveredVertex != nullptr) {
				rightMouseDownVertex = hoveredVertex;
				rightMouseDownVertex->pauseMove = true;
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Right) && rightMouseDownVertex != nullptr) {
				rightMouseDownVertex->pauseMove = false;
				rightMouseDownVertex = nullptr;
			}
		}

		void updateCamera() {
			if (isHovered)
				zoomTarget *= powf(1.05, ImGui::GetIO().MouseWheel);

			if (isHovered) {
				if (rightMouseDownVertex == nullptr) {
					if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, 0.0f)) {
						centerMapped.x += -ImGui::GetIO().MouseDelta.x / canvasDisplaySize / zoomLevel;
						centerMapped.y -= -ImGui::GetIO().MouseDelta.y / canvasDisplaySize / zoomLevel;
					}
				}

				centerMapped.x +=
						-(ImGui::GetIO().MousePos.x - centerDrawCoord.x
						  + (centerDrawCoord.x - ImGui::GetIO().MousePos.x)
						    * (powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevel;
				centerMapped.y +=
						(ImGui::GetIO().MousePos.y - centerDrawCoord.y
						 + (centerDrawCoord.y - ImGui::GetIO().MousePos.y)
						   * (powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevel;
			}

			// smoothy transition zoom level
			zoomLevel *= powf(zoomTarget / zoomLevel, 0.1f);
			zoomLevel = std::max(zoomTarget, FLT_MIN);
		}

		void updateMouseCursor() {
			if (isHovered) {
				if (hoveredVertex != nullptr || leftMouseDownVertex != nullptr || rightMouseDownVertex != nullptr)
					ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
				else
					ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			}
		}

		void drawGrid() {
			// TODO: Fix this monstrosity

			ImDrawList* drawList = ImGui::GetWindowDrawList();
			// Draw context 1*1 square
			drawList->AddRectFilled(
					ImVec2(
							centerDrawCoord.x - (centerMapped.x - 0.0f) * canvasDisplaySize * zoomLevel,
							centerDrawCoord.y + (centerMapped.y - 1.0f) * canvasDisplaySize * zoomLevel
					),
					ImVec2(
							centerDrawCoord.x - (centerMapped.x - 1.0f) * canvasDisplaySize * zoomLevel,
							centerDrawCoord.y + (centerMapped.y - 0.0f) * canvasDisplaySize * zoomLevel
					),
					IM_COL32(40, 40, 40, 255), 0.0f
			);

			{
				// vertical lines (secondary)
				const float contextGridSpacing = powf(10, -(int) log10f(zoomLevel) - 1);
				const float screenGridSpacing = contextGridSpacing * canvasDisplaySize * zoomLevel;
				const float centerSnapGridCoord =
						centerDrawCoord.x - (fmodf(centerMapped.x, contextGridSpacing) * canvasDisplaySize * zoomLevel);
				const float startingSnapGridCoord = centerSnapGridCoord - (int) (drawSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
				const float endingSnapGridCoord = centerSnapGridCoord + (int) (drawSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

				int lineDrawLimit = 500;
				for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
					drawList->AddLine(ImVec2(x, centerDrawCoord.y - drawSize.y / 2.0f), ImVec2(x, centerDrawCoord.y + drawSize.y / 2.0f),
					                  IM_COL32(90, 90, 90, 80), 1.0f);
					if (!(--lineDrawLimit))
						break;
				}
			}
			{
				// vertical lines (primary)
				const float contextGridSpacing = powf(10, -(int) log10f(zoomLevel));
				const float screenGridSpacing = contextGridSpacing * canvasDisplaySize * zoomLevel;
				const float centerSnapGridCoord =
						centerDrawCoord.x - (fmodf(centerMapped.x, contextGridSpacing) * canvasDisplaySize * zoomLevel);
				const float startingSnapGridCoord = centerSnapGridCoord - (int) (drawSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
				const float endingSnapGridCoord = centerSnapGridCoord + (int) (drawSize.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

				int lineDrawLimit = 500;
				for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
					drawList->AddLine(ImVec2(x, centerDrawCoord.y - drawSize.y / 2.0f), ImVec2(x, centerDrawCoord.y + drawSize.y / 2.0f),
					                  IM_COL32(90, 90, 90, 80), 2.0f);
					if (!(--lineDrawLimit))
						break;
				}
			}
			{
				// horizontal lines (secondary)
				const float contextGridSpacing = powf(10, -(int) log10f(zoomLevel) - 1);
				const float screenGridSpacing = contextGridSpacing * canvasDisplaySize * zoomLevel;
				const float centerSnapGridCoord =
						centerDrawCoord.y + (fmodf(centerMapped.y, contextGridSpacing) * canvasDisplaySize * zoomLevel);
				const float startingSnapGridCoord = centerSnapGridCoord - (int) (drawSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
				const float endingSnapGridCoord = centerSnapGridCoord + (int) (drawSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

				int lineDrawLimit = 500;
				for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
					drawList->AddLine(ImVec2(centerDrawCoord.x - drawSize.x / 2.0f, y), ImVec2(centerDrawCoord.x + drawSize.x / 2.0f, y),
					                  IM_COL32(90, 90, 90, 80), 1.0f);
					if (!(--lineDrawLimit))
						break;
				}
			}
			{
				// horizontal lines (primary)
				const float contextGridSpacing = powf(10, -(int) log10f(zoomLevel));
				const float screenGridSpacing = contextGridSpacing * canvasDisplaySize * zoomLevel;
				const float centerSnapGridCoord =
						centerDrawCoord.y + (fmodf(centerMapped.y, contextGridSpacing) * canvasDisplaySize * zoomLevel);
				const float startingSnapGridCoord = centerSnapGridCoord - (int) (drawSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
				const float endingSnapGridCoord = centerSnapGridCoord + (int) (drawSize.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

				int lineDrawLimit = 500;
				for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
					drawList->AddLine(ImVec2(centerDrawCoord.x - drawSize.x / 2.0f, y), ImVec2(centerDrawCoord.x + drawSize.x / 2.0f, y),
					                  IM_COL32(90, 90, 90, 80), 2.0f);
					if (!(--lineDrawLimit))
						break;
				}
			}


			ImVec2 contextOriginCenterDelta = ImVec2(centerDrawCoord.x - centerMapped.x * canvasDisplaySize * zoomLevel,
			                                         centerDrawCoord.y + centerMapped.y * canvasDisplaySize * zoomLevel);

			// blue origin dot
			drawList->AddCircleFilled(contextOriginCenterDelta, 5.0f, IM_COL32(0, 211, 255, 255));
		}

		void drawEdges(Graphene::Graph* graph) {
			// edge drawing
			Graphene::EdgeIter it(graph);
			while (it.next()) {
				ImGui::GetWindowDrawList()->AddLine(
						ImVec2(centerDrawCoord.x - (centerMapped.x - it.u->getCoord().x) * canvasDisplaySize * zoomLevel,
						       centerDrawCoord.y + (centerMapped.y - it.u->getCoord().y) * canvasDisplaySize * zoomLevel),
						ImVec2(centerDrawCoord.x - (centerMapped.x - it.v->getCoord().x) * canvasDisplaySize * zoomLevel,
						       centerDrawCoord.y + (centerMapped.y - it.v->getCoord().y) * canvasDisplaySize * zoomLevel),
						IM_COL32(200, 200, 200, 255), 5.0f * powf(zoomLevel, 0.1));
			}

			if (leftMouseDownVertex != nullptr) {
				if (hoveredVertex != nullptr) {
					if (graph->isAdjacent(leftMouseDownVertex->getNumber(), hoveredVertex->getNumber()) ||
					    graph->isAdjacent(hoveredVertex->getNumber(), leftMouseDownVertex->getNumber())) {
						// disconnecting vertices
						ImGui::GetWindowDrawList()->AddLine(
								getDrawCoord(leftMouseDownVertex->getCoord()),
								getDrawCoord(hoveredVertex->getCoord()),
								IM_COL32(200, 0, 0, 120), 10.0f * powf(zoomLevel, 0.1));
					} else {
						// connecting vertices
						ImGui::GetWindowDrawList()->AddLine(
								getDrawCoord(leftMouseDownVertex->getCoord()),
								getDrawCoord(hoveredVertex->getCoord()),
								IM_COL32(0, 255, 0, 120), 5.0f * powf(zoomLevel, 0.1));
					}
				} else {
					ImGui::GetWindowDrawList()->AddLine(
							getDrawCoord(leftMouseDownVertex->getCoord()),
							ImGui::GetIO().MousePos,
							IM_COL32(0, 255, 0, 60), 5.0f * powf(zoomLevel, 0.1));
				}
			}
		}

		void drawVertices(Graphene::Graph* graph) {
			Graphene::VertexIter it(graph);
			while (it.next()) {

				ImVec2 vertexScreenCoord(centerDrawCoord.x - (centerMapped.x - it.v->getCoord().x) * canvasDisplaySize * zoomLevel,
				                         centerDrawCoord.y + (centerMapped.y - it.v->getCoord().y) * canvasDisplaySize * zoomLevel);

				//ImGui::GetWindowDrawList()->AddCircleFilled(vertexScreenCoord, 20.0f * powf(zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f),
				//                                            (it.v == rightMouseDownVertex) ? IM_COL32(255, 221, 51, 255) : IM_COL32(255, 211, 0, 255));
				ImVec4 vertexFillCol(0.0f, 0.0f, 0.0f, 1.0f);
				ImVec4 vertexFillColHover(0.0f, 0.0f, 0.0f, 1.0f);
				ImGui::ColorConvertHSVtoRGB(it.v->connectedComponent / 8.0f, 0.9f, 0.8f, vertexFillCol.x, vertexFillCol.y, vertexFillCol.z);
				ImGui::ColorConvertHSVtoRGB(it.v->connectedComponent / 8.0f, 0.9f, 0.88f, vertexFillColHover.x, vertexFillColHover.y, vertexFillColHover.z);

				ImGui::GetWindowDrawList()->AddCircleFilled(
						vertexScreenCoord, 20.0f * powf(zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f),
						(it.v == rightMouseDownVertex) ? ImGui::ColorConvertFloat4ToU32(vertexFillColHover) : ImGui::ColorConvertFloat4ToU32(vertexFillCol));

				ImGui::PushFont(Gui::vertexTextFont);
				ImGui::SetWindowFontScale((36.0f / 54.0f) * powf(zoomLevel, 0.1) * ((it.v == rightMouseDownVertex) ? 1.1f : 1.0f));
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255));

				ImVec2 labelCenterPos(centerDrawCoord.x - (centerMapped.x - it.v->getCoord().x) * canvasDisplaySize * zoomLevel,
				                      centerDrawCoord.y + (centerMapped.y - it.v->getCoord().y) * canvasDisplaySize * zoomLevel);
				ImVec2 labelMinPos(labelCenterPos.x - 50.0f, labelCenterPos.y - 50.0f);
				ImVec2 labelMaxPos(labelCenterPos.x + 50.0f, labelCenterPos.y + 50.0f);
				std::string tempStr(std::to_string(it.v->getNumber()));
				char* label = new char[tempStr.length() + 1];
				strcpy(label, tempStr.c_str());
				ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
				ImVec2 labelAlign(0.5f, 0.5f);
				const ImRect bb(labelMinPos, labelMaxPos);

				ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, 0, &labelSize, labelAlign, &bb);

				ImGui::PopStyleColor(1);
				ImGui::SetWindowFontScale(1.0f);
				ImGui::PopFont();
			}

			if (hoveredVertex != nullptr) {
				ImGui::GetWindowDrawList()->AddCircle(
						ImVec2(centerDrawCoord.x - (centerMapped.x - hoveredVertex->getCoord().x) * canvasDisplaySize * zoomLevel,
						       centerDrawCoord.y + (centerMapped.y - hoveredVertex->getCoord().y) * canvasDisplaySize * zoomLevel
						),
						25.0f * powf(zoomLevel, 0.1), IM_COL32(150, 150, 255, 100), 0, 5.0f * powf(zoomLevel, 0.1));
			}
		}

		void show(Graphene::Core* core, Graphene::Graph* graph) {

			ImGui::SetNextWindowSizeConstraints(ImVec2(300, 350), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin(u8"Graph View", 0, ImGuiWindowFlags_NoCollapse);

			ImGui::Checkbox("Show grid", &gridVisible);
			ImGui::SameLine();
			ImGui::Checkbox("Auto adjust view (A)", &enableAutoAdjustView);

			if (!enableAutoAdjustView) {
				ImGui::SameLine();
				if (ImGui::Button("Reset view")) {
					zoomTarget = 1.0f;
					centerMapped = ImVec2(0.5f, 0.5f);
				}
			}

			canvasBegin();

			if (enableAutoAdjustView)
				autoAdjustView(graph);
			updateKeyboardShortcuts();

			updateHoveredVertex(core, graph);
			if (rightMouseDownVertex != nullptr) {
				rightMouseDownVertex->flushMove(0.0f);
				rightMouseDownVertex->move(
						Graphene::Vec2f(
								ImGui::GetIO().MouseDelta.x / canvasDisplaySize / zoomLevel,
								-ImGui::GetIO().MouseDelta.y / canvasDisplaySize / zoomLevel
						)
				);
				rightMouseDownVertex->flushMove(1.0f);
			}

			updateCamera();
			updateMouseCursor();

			drawGrid();

			drawEdges(graph);
			drawVertices(graph);

			framerateCounter.frameCount();

			ImGui::GetWindowDrawList()->AddText(
					ImVec2(topLeftDrawCoord.x + 5.0f, topLeftDrawCoord.y + 5.0f),
					ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.80, 0.81, 1.0f)),
					"Framerate", 0
			);
			ImGui::PushFont(Gui::framerateTextFont);
			ImGui::GetWindowDrawList()->AddText(
					ImVec2(topLeftDrawCoord.x + 5.0f, topLeftDrawCoord.y + 15.0f),
					ImGui::ColorConvertFloat4ToU32(ImVec4(0.59, 0.80, 0.81, 1.0f)),
					std::to_string(framerateCounter.getFramerate()).c_str(), 0
			);
			ImGui::PopFont();

			ImGui::GetWindowDrawList()->AddText(
					ImVec2(topLeftDrawCoord.x + 5.0f, topLeftDrawCoord.y + 40.0f),
					ImGui::ColorConvertFloat4ToU32(ImVec4(0.81, 0.72, 0.59, 1.0f)),
					"Graph update rate", 0
			);
			ImGui::PushFont(Gui::framerateTextFont);
			ImGui::GetWindowDrawList()->AddText(
					ImVec2(topLeftDrawCoord.x + 5.0f, topLeftDrawCoord.y + 50.0f),
					ImGui::ColorConvertFloat4ToU32(ImVec4(0.81, 0.72, 0.59, 1.0f)),
					std::to_string(core->updateRateCounter.getFramerate()).c_str(), 0
			);
			ImGui::PopFont();



			ImGui::GetWindowDrawList()->PopClipRect();

			canvasEnd();
		}
	}
}
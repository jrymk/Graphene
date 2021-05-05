#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "Include.h"
#include "../utils/ExceptionHandler.hpp"
#include "../graphene/Include.h"
#include "Themes.hpp"
#include <cstring>
#include <algorithm>

namespace Gui {

	namespace GraphView {

		ImVec2 sizePixelCoord;
		ImVec2 topLeftPixelCoord;
		ImVec2 centerPixelCoord;
		ImVec2 bottomRightPixelCoord;
		ImVec2 canvasMargin = ImVec2(100.0f, 100.0f);
		float canvasDisplaySize;

		bool autoZoomPan = true;
		bool showGrid = true;
		float zoomOffset = 1.0f;
		ImVec2 centerMappedCoord = ImVec2(0.5f, 0.5f);

		float zoomLevelRatio = 1.0f;
		bool useZoomTarget = false;
		float zoomLevelTarget = 1.0f;


		ImVec2 getPixelCoord(Graphene::Vec2f mappedCoord) {
			return { centerPixelCoord.x - (centerMappedCoord.x - mappedCoord.x) * canvasDisplaySize * zoomLevelRatio,
				centerPixelCoord.y + (centerMappedCoord.y - mappedCoord.y) * canvasDisplaySize * zoomLevelRatio };
		}


		void show(Graphene::Core* core, Graphene::Graph* graph) {

			ImGui::SetNextWindowSizeConstraints(ImVec2(300, 350), ImVec2(FLT_MAX, FLT_MAX));
			ImGui::Begin(u8"Graph View", 0, ImGuiWindowFlags_NoCollapse);

			bool contextMenuEnabled = false;
			ImGui::Checkbox("Show grid", &showGrid);
			ImGui::SameLine();
			ImGui::Checkbox("Auto adjust view (A)", &autoZoomPan);

			if (ImGui::IsKeyPressed('A', false))
				autoZoomPan = !autoZoomPan;

			sizePixelCoord = ImGui::GetContentRegionAvail();
			topLeftPixelCoord = ImGui::GetCursorScreenPos();
			centerPixelCoord = ImVec2(topLeftPixelCoord.x + sizePixelCoord.x / 2.0f, topLeftPixelCoord.y + sizePixelCoord.y / 2.0f);
			bottomRightPixelCoord = ImVec2(topLeftPixelCoord.x + sizePixelCoord.x, topLeftPixelCoord.y + sizePixelCoord.y);
			canvasDisplaySize = std::max(std::min(sizePixelCoord.x - canvasMargin.x * 2.0f, sizePixelCoord.y - canvasMargin.y * 2.0f), FLT_MIN);


			if (autoZoomPan) {
				ImGui::SameLine();
				ImGui::SliderFloat("Zoom multiplier", &zoomOffset, 0.0001f, 100.0f, 0, ImGuiSliderFlags_Logarithmic);
			}

			else {
				ImGui::SameLine();
				if (ImGui::Button("Reset view")) {
					zoomLevelTarget = 1.0f;
					useZoomTarget = true;
					zoomOffset = 0.5f;
					centerMappedCoord = ImVec2(0.5f, 0.5f);
				}
			}

			// catch mouse interactions
			ImGui::InvisibleButton("canvas", sizePixelCoord, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Mouse button held


			if (ImGui::IsItemHovered()) {

				// zoom level control
				if (!autoZoomPan)
					zoomLevelRatio *= powf(1.05, ImGui::GetIO().MouseWheel);
				else
					zoomOffset *= powf(1.05, ImGui::GetIO().MouseWheel);

				zoomLevelRatio = std::max(zoomLevelRatio, FLT_MIN);

				if (!autoZoomPan) {
					centerMappedCoord.x += -(ImGui::GetIO().MousePos.x - centerPixelCoord.x + (centerPixelCoord.x - ImGui::GetIO().MousePos.x) *
						(powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevelRatio;
					centerMappedCoord.y -= -(ImGui::GetIO().MousePos.y - centerPixelCoord.y + (centerPixelCoord.y - ImGui::GetIO().MousePos.y) *
						(powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevelRatio;
				}
			}

			// zoom target: zoom will smoothly transition to target value
			if (useZoomTarget) {
				zoomLevelRatio *= powf(zoomLevelTarget / zoomLevelRatio, 0.1);
				if (zoomLevelTarget / zoomLevelRatio < 1.01f && zoomLevelTarget / zoomLevelRatio > 1.0f / 1.01f) {
					useZoomTarget = false;
					zoomLevelRatio = zoomLevelTarget;
				}
			}


			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			drawList->PushClipRect(topLeftPixelCoord, bottomRightPixelCoord, true);

			// Draw border and background color
			drawList->AddRectFilled(topLeftPixelCoord, bottomRightPixelCoord, IM_COL32(0, 0, 0, 255), 6.0f, 0);


			float x_max = -1000000000.0;
			float x_min = 1000000000.0;
			float y_max = -1000000000.0;
			float y_min = 1000000000.0;

			for (int i = 0; i < graph->vertexCount; i++) {
				x_max = std::max(x_max, graph->vertices[i]->getCoord().x);
				x_min = std::min(x_min, graph->vertices[i]->getCoord().x);
				y_max = std::max(y_max, graph->vertices[i]->getCoord().y);
				y_min = std::min(y_min, graph->vertices[i]->getCoord().y);
			}

			if (autoZoomPan) {
				centerMappedCoord.x += ((x_max - x_min) / 2.0f + x_min - centerMappedCoord.x) * 0.3f;
				centerMappedCoord.y += ((y_max - y_min) / 2.0f + y_min - centerMappedCoord.y) * 0.3f;

				zoomLevelRatio += (1.0f / std::max(std::max(x_max - x_min, y_max - y_min), 0.1f) * zoomOffset - zoomLevelRatio) * 0.3f;
			}


			// Draw 1*1 square
			drawList->AddRectFilled(ImVec2(
				centerPixelCoord.x - (centerMappedCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio,
				centerPixelCoord.y + (centerMappedCoord.y - 1.0f) * canvasDisplaySize * zoomLevelRatio)
				, ImVec2(
					centerPixelCoord.x - (centerMappedCoord.x - 1.0f) * canvasDisplaySize * zoomLevelRatio,
					centerPixelCoord.y + (centerMappedCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio)
				, IM_COL32(40, 40, 40, 255), 0.0f);


			if (showGrid) {

				{
					// vertical lines (secondary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio) - 1);
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerPixelCoord.x - (fmodf(centerMappedCoord.x, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizePixelCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizePixelCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					int lineDrawLimit = 500;
					for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
						drawList->AddLine(ImVec2(x, centerPixelCoord.y - sizePixelCoord.y / 2.0f), ImVec2(x, centerPixelCoord.y + sizePixelCoord.y / 2.0f), IM_COL32(90, 90, 90, 80), 1.0f);
						if (!(--lineDrawLimit))
							break;
					}
				}
				{
					// vertical lines (primary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio));
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerPixelCoord.x - (fmodf(centerMappedCoord.x, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizePixelCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizePixelCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					int lineDrawLimit = 500;
					for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing) {
						drawList->AddLine(ImVec2(x, centerPixelCoord.y - sizePixelCoord.y / 2.0f), ImVec2(x, centerPixelCoord.y + sizePixelCoord.y / 2.0f), IM_COL32(90, 90, 90, 80), 2.0f);
						if (!(--lineDrawLimit))
							break;
					}
				}
				{
					// horizontal lines (secondary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio) - 1);
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerPixelCoord.y + (fmodf(centerMappedCoord.y, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizePixelCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizePixelCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					int lineDrawLimit = 500;
					for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
						drawList->AddLine(ImVec2(centerPixelCoord.x - sizePixelCoord.x / 2.0f, y), ImVec2(centerPixelCoord.x + sizePixelCoord.x / 2.0f, y), IM_COL32(90, 90, 90, 80), 1.0f);
						if (!(--lineDrawLimit))
							break;
					}
				}
				{
					// horizontal lines (primary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio));
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerPixelCoord.y + (fmodf(centerMappedCoord.y, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizePixelCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizePixelCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					int lineDrawLimit = 500;
					for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing) {
						drawList->AddLine(ImVec2(centerPixelCoord.x - sizePixelCoord.x / 2.0f, y), ImVec2(centerPixelCoord.x + sizePixelCoord.x / 2.0f, y), IM_COL32(90, 90, 90, 80), 2.0f);
						if (!(--lineDrawLimit))
							break;
					}
				}


				ImVec2 contentOriginCenterDelta = ImVec2(centerPixelCoord.x - centerMappedCoord.x * canvasDisplaySize * zoomLevelRatio,
					centerPixelCoord.y + centerMappedCoord.y * canvasDisplaySize * zoomLevelRatio);


				// blue origin dot
				drawList->AddCircleFilled(contentOriginCenterDelta, 5.0f, IM_COL32(0, 211, 255, 255));


				Graphene::Vertex* hoverVertex = nullptr;
				static Graphene::Vertex* edgeAddVertex;
				static bool addingEdge = false;

				float closestVertexDistance = FLT_MAX;

				for (int i = 0; i < graph->vertexCount; i++) {
					ImVec2 vertexScreenCoord(centerPixelCoord.x - (centerMappedCoord.x - graph->vertices[i]->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
						centerPixelCoord.y + (centerMappedCoord.y - graph->vertices[i]->getCoord().y) * canvasDisplaySize * zoomLevelRatio);
					float mouseVertexDistanceSquared = powf(ImGui::GetIO().MousePos.x - vertexScreenCoord.x, 2.0f) + powf(ImGui::GetIO().MousePos.y - vertexScreenCoord.y, 2.0f);

					if (ImGui::IsItemHovered()) {
						if (mouseVertexDistanceSquared <= powf(40.0f * powf(zoomLevelRatio, 0.1), 2.0f)) {
							if (mouseVertexDistanceSquared <= closestVertexDistance) {
								hoverVertex = graph->vertices[i];
								closestVertexDistance = mouseVertexDistanceSquared;
							}
						}
					}
				}

				for (int i = 0; i < graph->edgeCount; i++) {
					drawList->AddLine(
						ImVec2(centerPixelCoord.x - (centerMappedCoord.x - graph->edges[i].startingVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
							centerPixelCoord.y + (centerMappedCoord.y - graph->edges[i].startingVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
						ImVec2(centerPixelCoord.x - (centerMappedCoord.x - graph->edges[i].endingVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
							centerPixelCoord.y + (centerMappedCoord.y - graph->edges[i].endingVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
						IM_COL32(200, 200, 200, 255), 5.0f * powf(zoomLevelRatio, 0.1));
				}


				// pan control
				const float mousePanThreshold = contextMenuEnabled ? -1.0f : 0.0f;

				/*if (hoverVertex != nullptr && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mousePanThreshold)) {
					hoverVertex->move(
						Graphene::Vec2f(
							ImGui::GetIO().MouseDelta.x / canvasDisplaySize / zoomLevelRatio,
							-ImGui::GetIO().MouseDelta.y / canvasDisplaySize / zoomLevelRatio
						)
					);
					hoverVertex->flushMove(1.0f);
				}*/

				static Graphene::Vertex* draggingVertex = nullptr;
				static ImVec2 dragVertexDownPos;
				static bool isDraggingVertex = false;

				if (hoverVertex != nullptr && !isDraggingVertex && ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
					dragVertexDownPos = ImGui::GetIO().MousePos;
					isDraggingVertex = true;
					draggingVertex = hoverVertex;
				}

				if (isDraggingVertex) {
					draggingVertex->move(
						Graphene::Vec2f(
							(ImGui::GetIO().MousePos.x - dragVertexDownPos.x) / canvasDisplaySize / zoomLevelRatio,
							-(ImGui::GetIO().MousePos.y - dragVertexDownPos.y) / canvasDisplaySize / zoomLevelRatio
						)
					);
					draggingVertex->flushMove(1.0f);
					dragVertexDownPos = ImGui::GetIO().MousePos;
				}

				if (isDraggingVertex && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
					isDraggingVertex = false;
				}

				bool deleteEdge = false;

				if (hoverVertex != nullptr) {
					for (std::vector<Graphene::Edge>::iterator e = graph->edges.begin(); e != graph->edges.end(); e++) {
						if ((e->startingVertex == edgeAddVertex && e->endingVertex == hoverVertex) || (e->startingVertex == hoverVertex && e->endingVertex == edgeAddVertex)) {
							deleteEdge = true;
							break;
						}
					}
				}


				if (addingEdge && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					addingEdge = false;

					if (hoverVertex != nullptr) {
						if (!deleteEdge) {
							graph->edgeCount++;
							graph->edges.emplace_back(edgeAddVertex, hoverVertex, false);
							graph->adjList[edgeAddVertex->getNumber()].emplace_back(hoverVertex);
							graph->adjList[hoverVertex->getNumber()].emplace_back(edgeAddVertex);

							core->pendingInputUpdate = true;
						}


						else {
							for (std::vector<Graphene::Edge>::iterator e = graph->edges.begin(); e != graph->edges.end(); e++) {
								if ((e->startingVertex == edgeAddVertex && e->endingVertex == hoverVertex) || (e->startingVertex == hoverVertex && e->endingVertex == edgeAddVertex)) {
									graph->edgeCount--;
									graph->edges.erase(e);
									graph->adjList[edgeAddVertex->getNumber()].erase(
										std::find(graph->adjList[edgeAddVertex->getNumber()].begin(), graph->adjList[edgeAddVertex->getNumber()].end(), hoverVertex)
									);
									graph->adjList[hoverVertex->getNumber()].erase(
										std::find(graph->adjList[hoverVertex->getNumber()].begin(), graph->adjList[hoverVertex->getNumber()].end(), edgeAddVertex)
									);

									core->pendingInputUpdate = true;

									break;
								}
							}
						}
					}
				}


				if (addingEdge) {
					if (deleteEdge) {
						drawList->AddLine(
							ImVec2(centerPixelCoord.x - (centerMappedCoord.x - edgeAddVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
								centerPixelCoord.y + (centerMappedCoord.y - edgeAddVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
							ImVec2(centerPixelCoord.x - (centerMappedCoord.x - hoverVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
								centerPixelCoord.y + (centerMappedCoord.y - hoverVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
							IM_COL32(200, 0, 0, 255), 5.0f * powf(zoomLevelRatio, 0.1));
					}
					else {
						if (hoverVertex != nullptr) {
							drawList->AddLine(
								ImVec2(centerPixelCoord.x - (centerMappedCoord.x - edgeAddVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
									centerPixelCoord.y + (centerMappedCoord.y - edgeAddVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
								ImVec2(centerPixelCoord.x - (centerMappedCoord.x - hoverVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
									centerPixelCoord.y + (centerMappedCoord.y - hoverVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
								IM_COL32(0, 200, 0, 255), 5.0f * powf(zoomLevelRatio, 0.1));
						}
						else {
							drawList->AddLine(
								ImVec2(centerPixelCoord.x - (centerMappedCoord.x - edgeAddVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
									centerPixelCoord.y + (centerMappedCoord.y - edgeAddVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio),
								ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y),
								IM_COL32(0, 200, 0, 255), 5.0f * powf(zoomLevelRatio, 0.1));
						}
					}
				}



				if (!isDraggingVertex && !autoZoomPan) {
					if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mousePanThreshold)) {
						centerMappedCoord.x += -ImGui::GetIO().MouseDelta.x / canvasDisplaySize / zoomLevelRatio;
						centerMappedCoord.y -= -ImGui::GetIO().MouseDelta.y / canvasDisplaySize / zoomLevelRatio;
					}
				}



				if (ImGui::IsItemHovered()) {
					if (hoverVertex != nullptr || isDraggingVertex || addingEdge)
						ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
					else
						ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
				}


				for (int i = 0; i < graph->vertexCount; i++) {
					ImVec2 vertexScreenCoord(centerPixelCoord.x - (centerMappedCoord.x - graph->vertices[i]->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
						centerPixelCoord.y + (centerMappedCoord.y - graph->vertices[i]->getCoord().y) * canvasDisplaySize * zoomLevelRatio);

					drawList->AddCircleFilled(vertexScreenCoord, 20.0f * powf(zoomLevelRatio, 0.1), IM_COL32(255, 211, 0, 255));

					ImGui::PushFont(Gui::vertexTextFont);
					ImGui::SetWindowFontScale((36.0f / 54.0f) * powf(zoomLevelRatio, 0.1));
					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(15, 15, 15, 255));

					ImVec2 labelCenterPos(centerPixelCoord.x - (centerMappedCoord.x - graph->vertices[i]->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
						centerPixelCoord.y + (centerMappedCoord.y - graph->vertices[i]->getCoord().y) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 labelMinPos(labelCenterPos.x - 50.0f, labelCenterPos.y - 50.0f);
					ImVec2 labelMaxPos(labelCenterPos.x + 50.0f, labelCenterPos.y + 50.0f);
					std::string tempStr(std::to_string(i));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(0.5f, 0.5f);
					const ImRect bb(labelMinPos, labelMaxPos);

					ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, 0, &labelSize, labelAlign, &bb);

					ImGui::PopStyleColor(1);
					ImGui::SetWindowFontScale(1.0f);
					ImGui::PopFont();
				}



				if (hoverVertex != nullptr) {
					drawList->AddCircle(ImVec2(centerPixelCoord.x - (centerMappedCoord.x - hoverVertex->getCoord().x) * canvasDisplaySize * zoomLevelRatio,
						centerPixelCoord.y + (centerMappedCoord.y - hoverVertex->getCoord().y) * canvasDisplaySize * zoomLevelRatio)
						, 25.0f * powf(zoomLevelRatio, 0.1), IM_COL32(150, 150, 255, 100), 0, 5.0f * powf(zoomLevelRatio, 0.1));

					if (!addingEdge && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
						addingEdge = true;
						edgeAddVertex = hoverVertex;
					}


				}


				{
					// x positive 10x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio + 2.0f,
						centerPixelCoord.y + (centerMappedCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(0.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// x negative 10x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x + powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio - 52.0f,
						centerPixelCoord.y + (centerMappedCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// x positive 1x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio + 2.0f,
						centerPixelCoord.y + (centerMappedCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(0.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// x negative 1x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x + powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio - 52.0f,
						centerPixelCoord.y + (centerMappedCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// y positive 10x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerPixelCoord.y + (centerMappedCoord.y - powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio - 32.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 1.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// y negative 10x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerPixelCoord.y + (centerMappedCoord.y + powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio + 2.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// y positive 1x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerPixelCoord.y + (centerMappedCoord.y - powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio - 32.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 1.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// y negative 1x
					ImVec2 gridCoordLabelMinPos(centerPixelCoord.x - (centerMappedCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerPixelCoord.y + (centerMappedCoord.y + powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio + 2.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}


			}

			if (!autoZoomPan)
				drawList->AddText(ImVec2(topLeftPixelCoord.x + 4.0f, bottomRightPixelCoord.y - 36.0f), IM_COL32(200, 200, 200, 200), "Pan: Hold right mouse button   Zoom: Scroll", 0);
			else
				drawList->AddText(ImVec2(topLeftPixelCoord.x + 4.0f, bottomRightPixelCoord.y - 36.0f), IM_COL32(200, 200, 200, 200), "Auto adjust view enabled, manual pan and zoom is disabled", 0);

			drawList->AddText(ImVec2(topLeftPixelCoord.x + 4.0f, bottomRightPixelCoord.y - 20.0f), IM_COL32(200, 200, 200, 200), "Drag from vertex to vertex with left mouse button to create/delete edge, drag vertex with right mouse button to nudge vertex position", 0);

			drawList->PopClipRect();

			ImGui::End();
		}

	}

}
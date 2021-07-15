
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "Common.hpp"
#include "../../../../Dependencies/imgui/imgui.h"
#include "../../../core/BlockCutTree.hpp"
#include "ConvexHull.hpp"
#include "View.hpp"
#include "AlignedText.hpp"
#include "DrawPolygon.hpp"

namespace graphene::editor::graphview::renderer {

void drawGridLines(bool vertical, double origin, double spacing, float perpBegin, float perpEnd, float parBegin,
				   float parEnd, ImColor col, float thickness) {
	const float canvas = vertical ? view::canvasOrigin.x : view::canvasOrigin.y;
	const double snapGrid = origin + round((canvas - origin) / spacing) * spacing;
	int lineDrawLimit = 500;
	for (int i = int(perpBegin / spacing); i <= int(perpEnd / spacing); i++) {
		if (vertical) {
			ImGui::GetWindowDrawList()->AddLine(ImVec2(float(snapGrid + i * spacing), view::canvasOrigin.y + parBegin),
												ImVec2(float(snapGrid + i * spacing), view::canvasOrigin.y + parEnd),
												col, thickness);
		} else {
			ImGui::GetWindowDrawList()->AddLine(ImVec2(view::canvasOrigin.x + parBegin, float(snapGrid + i * spacing)),
												ImVec2(view::canvasOrigin.x + parEnd, float(snapGrid + i * spacing)),
												col, thickness);
		}
		if (!(--lineDrawLimit))
			break;
	}
}

void drawGrid() {
	drawGridLines(true, graphene::editor::graphview::view::mapToCanvas(0.0, 0.0).x,
				  graphene::editor::graphview::view::mapToCanvas(pow(10, -(int)ceil(log10(view::zoomLevel)))),
				  -view::canvasSize.x / 2.0f, view::canvasSize.x / 2.0f, -view::canvasSize.y / 2.0f,
				  view::canvasSize.y / 2.0f, IM_COL32(90, 90, 90, 80), 2.0);
	drawGridLines(true, graphene::editor::graphview::view::mapToCanvas(0.0, 0.0).x,
				  graphene::editor::graphview::view::mapToCanvas(pow(10, -(int)ceil(log10(view::zoomLevel)) - 1)),
				  -view::canvasSize.x / 2.0f, view::canvasSize.x / 2.0f, -view::canvasSize.y / 2.0f,
				  view::canvasSize.y / 2.0f, IM_COL32(90, 90, 90, 80), 1.0);
	drawGridLines(false, graphene::editor::graphview::view::mapToCanvas(0.0, 0.0).y,
				  graphene::editor::graphview::view::mapToCanvas(pow(10, -(int)ceil(log10(view::zoomLevel)))),
				  -view::canvasSize.y / 2.0f, view::canvasSize.y / 2.0f, -view::canvasSize.x / 2.0f,
				  view::canvasSize.x / 2.0f, IM_COL32(90, 90, 90, 80), 2.0);
	drawGridLines(false, graphene::editor::graphview::view::mapToCanvas(0.0, 0.0).y,
				  graphene::editor::graphview::view::mapToCanvas(pow(10, -(int)ceil(log10(view::zoomLevel)) - 1)),
				  -view::canvasSize.y / 2.0f, view::canvasSize.y / 2.0f, -view::canvasSize.x / 2.0f,
				  view::canvasSize.x / 2.0f, IM_COL32(90, 90, 90, 80), 1.0);
}

/*void drawComponents() {
	for (auto& c : Graphene::core->getGraphObj()->components) {

		::Gui::ConvexHull compConvexHull;
		for (auto& v : c->adjList) {
			for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 8.0f) {
				compConvexHull.newPoint(
					{v.first->getCoord().x +
						 View::mapToContext(float(20.0 * pow(view::zoomLevel, 0.1) + 30.0f) * cos(angle)),
					 v.first->getCoord().y +
						 View::mapToContext(float(20.0 * pow(view::zoomLevel, 0.1) + 30.0f) * sin(angle))});
			}
		}
		compConvexHull.build();

		std::vector<ImVec2> poly;
		for (auto& p : compConvexHull.output)
			poly.emplace_back(graphene::editor::graphview::view::mapToCanvas(p));

		Gui::drawPolygon(poly, IM_COL32(100, 100, 100, 100));

		int bccId = 0;
		for (auto& bcc : c->blockCutTree->mapping) {
			::Gui::ConvexHull convexHull;

			ImVec4 col(0.0f, 0.0f, 0.0f, 0.30f);
			ImVec4 colHsv(float(float(bccId) / c->blockCutTree->mapping.size()), 0.5f, 1.0f, 0.0f);
			ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);

			for (auto& v : bcc.second) {
				for (double angle = 0; angle < 2 * M_PI; angle += M_PI / 8.0f) {
					convexHull.newPoint(
						{v->getCoord().x +
							 View::mapToContext(float(20.0 * pow(view::zoomLevel, 0.1) + 15.0f) * cos(angle)),
						 v->getCoord().y +
							 View::mapToContext(float(20.0 * pow(view::zoomLevel, 0.1) + 15.0f) * sin(angle))});
				}
			}
			convexHull.build();

			std::vector<ImVec2> poly;
			for (auto& p : convexHull.output)
				poly.emplace_back(graphene::editor::graphview::view::mapToCanvas(p));

			Gui::drawPolygon(poly, ImGui::ColorConvertFloat4ToU32(col));

			bccId++;
		}
	}

	if (Controls::hoveredComponent && !Controls::hoveredVertex && !Controls::leftMouseDownVertex &&
		!Controls::rightMouseDownVertex) {*/
/*ImGui::GetWindowDrawList()->AddCircle(
		graphene::editor::graphview::view::mapToCanvas(Controls::hoveredComponent->center),
		float(Controls::hoveredComponent->radius * view::canvasFrameSize * view::zoomLevel + 50.0 *
pow(view::zoomLevel, 0.1)), IM_COL32(150, 150, 255, 100), 0, 5.0f
);*/
/*ImVec4 col(0.0f, 0.0f, 0.0f, 0.28f);
ImVec4 colHsv(0.0f, 0.0f, 0.0f, 0.0f);
ImGui::ColorConvertRGBtoHSV(Controls::hoveredComponent->color.x, Controls::hoveredComponent->color.y,
Controls::hoveredComponent->color.z, colHsv.x, colHsv.y, colHsv.z); colHsv.y = colHsv.y * 0.5f;
ImGui::ColorConvertHSVtoRGB(colHsv.x, colHsv.y, colHsv.z, col.x, col.y, col.z);

ImGui::GetWindowDrawList()->AddCircleFilled(
		graphene::editor::graphview::view::mapToCanvas(Controls::hoveredComponent->center),
		float(Controls::hoveredComponent->radius * view::canvasFrameSize * view::zoomLevel + 50.0 *
pow(view::zoomLevel, 0.1)), ImGui::ColorConvertFloat4ToU32(col),
		0
);*//*
	}
}*/

void drawEdges() {
	// edge drawing
	for (auto& e : graphene::editor::graphic::frontBuffer->edges) {
		ImGui::GetWindowDrawList()->AddLine(graphene::editor::graphview::view::mapToCanvas(e.position1),
											graphene::editor::graphview::view::mapToCanvas(e.position2),
											IM_COL32(200, 200, 200, 255), float(5.0 * pow(view::zoomLevel, 0.1)));
	}
}

void drawVertices() {
	for (auto& v : graphene::editor::graphic::frontBuffer->vertices) {
		ImVec2 vertexScreenCoord(float(view::canvasOrigin.x - (view::centerContext.x - v.position.x) *
																  view::canvasFrameSize * view::zoomLevel),
								 float(view::canvasOrigin.y + (view::centerContext.y - v.position.y) *
																  view::canvasFrameSize * view::zoomLevel));

		// ImGui::GetWindowDrawList()->AddCircleFilled(vertexScreenCoord, 20.0f * powf(view::zoomLevel, 0.1) *
		// ((it.v == Controls::rightMouseDownVertex) ? 1.1f : 1.0f),
		//                                            (it.v == Controls::rightMouseDownVertex) ? IM_COL32(255, 221,
		//                                            51, 255) : IM_COL32(255, 211, 0, 255));
	}
}

} // namespace graphene::editor::graphview::renderer
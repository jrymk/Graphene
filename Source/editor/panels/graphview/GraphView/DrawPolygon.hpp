#pragma once

#include <vector>
#include "Triangulate.hpp"
#include "../../../../tools/Log.hpp"

namespace Gui {
void drawPolygon(const std::vector<ImVec2>& poly, ImU32 col) {
	std::vector<ImVec2> result;
	Triangulate::process(poly, result);
	for (int i = 0; i < result.size(); i += 3) {
		ImGui::GetWindowDrawList()->AddTriangleFilled(result[i], result[i + 1], result[i + 2], col);
	}
}

bool isInsidePoly(const std::vector<ImVec2>& poly, ImVec2 p) {
	std::vector<ImVec2> result;
	Triangulate::process(poly, result);
	for (int i = 0; i < result.size(); i += 3) {
		if (Triangulate::isInside(result[i], result[i + 1], result[i + 2], p))
			return true;
	}
	return false;
}

/*enum LineCornerStyle {
	BEVEL,
	ROUND,
	MITER,
};
void drawOutline(const std::vector<ImVec2> &poly, float stroke, LineCornerStyle corner, ImU32 col) {
	for (int i = 0; i < poly.size(); i++) {
		auto u = poly[i];
		auto v = poly[i == poly.size() - 1 ? 0 : i + 1];
		float len = powf((v.x - u.x) * (v.x - u.x) + (v.y - u.y) * (v.y - u.y), 0.5f);
		ImVec2 vec = {v.x - u.x, v.y - u.y};
		auto normal = {vec.x / len, vec.y / len};
		switch (corner) {
			case LineCornerStyle::BEVEL:
				ImGui::GetWindowDrawList()->AddTriangleFilled(
						col
				);
		}
	}
}*/

} // namespace Gui
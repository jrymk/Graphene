#pragma once

#include <vector>
#include <algorithm>
#include "../../../../Dependencies/imgui/imgui.h"
#include "../../../core/Vertex.hpp"
#include "../../../core/Edge.hpp"

namespace Gui {
class ConvexHull {
	std::vector<Vec2f> points;
	std::vector<Vec2f> sortedPoints;

  public:
	std::vector<Vec2f> output;

	ConvexHull() = default;

	void newPoint(Vec2f p) { points.emplace_back(p); }

	void build(bool containOnEdge = false) {
		double eps = 1e-9;

		sortedPoints = points;
		sort(sortedPoints.begin(), sortedPoints.end());

		output.clear();
		output.reserve((int)sortedPoints.size());

		for (int rep = 0; rep < 2; rep++) {
			int sz = (int)output.size();

			for (auto& point : sortedPoints) {
				while ((int)output.size() - sz >= 2) {
					Vec2f back = output.back();
					Vec2f preBack = output[(int)output.size() - 2];
					double cross = (back - preBack) ^ (point - preBack);

					if (cross < 0 || (!containOnEdge && abs(cross) <= eps)) {
						output.pop_back();
					} else {
						break;
					}
				}
				output.emplace_back(point);
			}

			output.pop_back();
			reverse(sortedPoints.begin(), sortedPoints.end());
		}
	}
};
} // namespace Gui
#pragma once

#include <vector>
#include "../../../core/Vector.hpp"

class Triangulate {
  public:
	static bool process(const std::vector<ImVec2>& contour, std::vector<ImVec2>& result) {
		int n = contour.size();
		if (n < 3)
			return false;
		int* V = new int[n];

		if (0.0f < area(contour))
			for (int v = 0; v < n; v++)
				V[v] = v;
		else
			for (int v = 0; v < n; v++)
				V[v] = (n - 1) - v;
		int nv = n;
		int count = 2 * nv;

		for (int m = 0, v = nv - 1; nv > 2;) {
			if (0 >= (count--))
				return false;

			int u = v;
			if (nv <= u)
				u = 0; /* previous */
			v = u + 1;
			if (nv <= v)
				v = 0; /* new v    */
			int w = v + 1;
			if (nv <= w)
				w = 0; /* next     */

			if (snip(contour, u, v, w, nv, V)) {
				int a, b, c, s, t;
				a = V[u];
				b = V[v];
				c = V[w];
				result.push_back(contour[a]);
				result.push_back(contour[b]);
				result.push_back(contour[c]);
				m++;
				for (s = v, t = v + 1; t < nv; s++, t++)
					V[s] = V[t];
				nv--;
				count = 2 * nv;
			}
		}
		delete[] V;
		return true;
	}

	static float area(const std::vector<ImVec2>& contour) {
		int n = contour.size();
		float A = 0.0f;
		for (int p = n - 1, q = 0; q < n; p = q++)
			A += contour[p].x * contour[q].y - contour[q].x * contour[p].y;
		return A * 0.5f;
	}

	static bool isInside(ImVec2 a, ImVec2 b, ImVec2 c, ImVec2 p) {
		return ((c.x - b.x) * (p.y - b.y) - (c.y - b.y) * (p.x - b.x) >= 0.0f &&
				(b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x) >= 0.0f &&
				(a.x - c.x) * (p.y - c.y) - (a.y - c.y) * (p.x - c.x) >= 0.0f);
	};

  private:
	static bool snip(const std::vector<ImVec2>& contour, int u, int v, int w, int n, int* V) {
		int i;
		ImVec2 a(contour[V[u]].x, contour[V[u]].y);
		ImVec2 b(contour[V[v]].x, contour[V[v]].y);
		ImVec2 c(contour[V[w]].x, contour[V[w]].y);

		if (1E-9 > ((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x)))
			return false;

		for (i = 0; i < n; i++) {
			if ((i == u) || (i == v) || (i == w))
				continue;
			ImVec2 p(contour[V[i]].x, contour[V[i]].y);
			if (isInside(a, b, c, p))
				return false;
		}
		return true;
	}
};
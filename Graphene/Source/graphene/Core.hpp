#pragma once
#define NOMINMAX
#define _USE_MATH_DEFINES
#include <windows.h>
#include <math.h>
#include <iostream>
#include <random>
#include <map>
#include <iomanip>
#include "Graph.hpp"
#include "Structure.hpp"

namespace Graphene {

	class Core {

	public:

		Graph* m_graph;

		Core(Graph& graph) {
			m_graph = &graph;
		}

		float m_c1 = 2;
		float m_c2 = 1;
		float m_c3 = 1;
		float m_c4 = 0.1;

		bool pendingInputUpdate = false;

		Graph* getGraph() {
			return m_graph;
		}


		float distance(Vertex* u, Vertex* v) {
			float dx = u->getCoord().x - v->getCoord().x;
			float dy = u->getCoord().y - v->getCoord().y;
			return sqrt(dx * dx + dy * dy);
		}

		float length(Vec2f a) {
			return sqrt(a.x * a.x + a.y * a.y);
		}

		Vec2f attractForce(Vertex* u, Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Vec2f(0, 0);
			float coeff = m_c1 * log(dis / m_c2);
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}

		Vec2f repelForce(Vertex* u, Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Vec2f(0, 0);
			float coeff = -m_c3 / dis / dis;
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}




		void updatePos() {

			Vec2f total(0, 0);
			for (auto u : m_graph->vertices) {

				for (auto v : m_graph->vertices) {

					//std::cerr << "r " << u->getNumber() << " " << v->getNumber() << " " << repelForce(u, v) << std::endl;
					//		<< distance(u, v) << " " << (v->getCoord() - u->getCoord()).normalize() << std::endl;
					if (u->getNumber() != 0) u->move(repelForce(u, v));
					else total += repelForce(u, v);

				}

				for (auto v : m_graph->vertices) {
					if (!m_graph->isAdjacent(u->getNumber(), v->getNumber())) continue;
					float dis = distance(u, v);
					//std::cerr << "a " << u->getNumber() << " " << v->getNumber() << " " << attractForce(u, v) << std::endl;
					//std::cerr << dis << " " << m_c1 << " " << m_c2 << " " << dis / m_c2 << " " << log(dis / m_c2) << std::endl;
					if (u->getNumber() != 0) u->move(attractForce(u, v));
					else total += attractForce(u, v);
				}

			}

			total *= -1;

			for (auto u : m_graph->vertices) {

				if (u->getNumber() == 0) continue;
				u->move(total);
				u->flushMove(m_c4);

			}

		}

	};

};

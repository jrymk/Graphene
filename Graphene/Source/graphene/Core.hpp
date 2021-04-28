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

	/***************************************************************************************************************/
	// Handles all the graph calculations

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

		bool not_adjacent_repel = true;

		float distance(Structure::Vertex* u, Structure::Vertex* v) {
			float dx = u->getCoord().x - v->getCoord().x;
			float dy = u->getCoord().y - v->getCoord().y;
			return sqrt(dx * dx + dy * dy);
		}

		float length(Structure::Vec2f a) {
			return sqrt(a.x * a.x + a.y * a.y);
		}

		Structure::Vec2f attractForce(Structure::Vertex* u, Structure::Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Structure::Vec2f(0, 0);
			float coeff = m_c1 * log(dis / m_c2);
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}

		Structure::Vec2f repelForce(Structure::Vertex* u, Structure::Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Structure::Vec2f(0, 0);
			float coeff = -m_c3 / dis / dis;
			return (v->getCoord() - u->getCoord()).normalize() * coeff;
		}




		void updatePos() {
			std::cerr << "test" << std::endl;
			Structure::Vec2f total(0, 0);
			for (auto u : m_graph->vertices) {
				for(auto v : m_graph->vertices) {
					if (!m_graph->isAdjacent(u->getNumber(), v->getNumber()) || not_adjacent_repel) {
						//std::cerr << "r " << u->getNumber() << " " << v->getNumber() << " " << repelForce(u, v) << std::endl;
						//		<< distance(u, v) << " " << (v->getCoord() - u->getCoord()).normalize() << std::endl;
						if (u->getNumber() != 0) u->move(repelForce(u, v));
						else total += repelForce(u, v);
					}
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

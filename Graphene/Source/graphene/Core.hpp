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
			return (v->getCoord() - u->getCoord()) * coeff;
		}

		Structure::Vec2f repelForce(Structure::Vertex* u, Structure::Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Structure::Vec2f(0, 0);
			float coeff = -m_c3 / dis / dis;
			return (v->getCoord() - u->getCoord()) * coeff;
		}

		void updatePos() {
			for (auto u : m_graph->vertices) {
				for(auto v : m_graph->vertices) {
					if (!m_graph->isAdjacent(u->getNumber(), v->getNumber()) || not_adjacent_repel) {
						//std::cerr << u->getNumber() << " " << v->getNumber() << " " << repelForce(u, v) << std::endl;
						u->move(repelForce(u, v));
					}
				}
				for (auto v : m_graph->adjList[u->getNumber()]) {
					//if(((*v)->number == 0 && (*u)->number == 1) || ((*v)->number == 1 && (*u)->number == 0))
					//std::cerr << "a " << (*u)->number << " " << (*v)->number << " " << attractForce(*u, *v)
							//<< " " << repelForce(*u, *v) << " " << distance(*u, *v) << std::endl;
					u->move(attractForce(u, v));
				}
			}
			for (auto u : m_graph->vertices) {
				u->flushMove(m_c4);
			}
		}

	};

};

#pragma once
#define _USE_MATH_DEFINES
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

		static float genRandom() {
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_real_distribution<> dis(0.0, 1.0);
			return dis(gen);
		}

		void printPos(int decimalPlaces) {
			std::cout << std::fixed << std::setprecision(decimalPlaces);
			std::cout << "----------------------------------\n";
			for (int i = 0; i < m_graph->vertexCount; i++) {
				std::cout << "Vertex[ " << i << " ] position\t" << m_graph->vertices[i]->coord << '\n';
			}
			std::cout << "----------------------------------\n";
		}

		float m_c1 = 2;
		float m_c2 = 1;
		float m_c3 = 1;
		float m_c4 = 0.1;

		bool not_adjacent_repel = true;

		float distance(Structure::Vertex* u, Structure::Vertex* v) {
			float dx = u->coord.x - v->coord.x;
			float dy = u->coord.y - v->coord.y;
			return sqrt(dx * dx + dy * dy);
		}

		float length(Structure::Vec2f a) {
			return sqrt(a.x * a.x + a.y * a.y);
		}

		Structure::Vec2f attractForce(Structure::Vertex* u, Structure::Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Structure::Vec2f(0, 0);
			float coeff = m_c1 * log10f(std::max(1.0f, dis / m_c2));
			return (v->coord - u->coord) * coeff;
		}

		Structure::Vec2f repelForce(Structure::Vertex* u, Structure::Vertex* v) {
			float dis = distance(u, v);
			if (dis == 0.0)
				return Structure::Vec2f(0, 0);
			float coeff = -m_c3 / dis / dis;
			return (v->coord - u->coord) * coeff;
		}

		void initializePos() {
			for (int i = 0; i < (int)m_graph->vertices.size(); i++) {
				m_graph->vertices[i]->coord = Structure::Vec2f(genRandom(), genRandom());
			}
		}

		bool isAdjacent(int u, int v) {
			if (std::find(m_graph->adjList[u].begin(), m_graph->adjList[u].end(), m_graph->vertices[v]) != m_graph->adjList[u].end())
				return true;
			return false;
		}

		void updatePos() {
			for (std::vector<Structure::Vertex*>::iterator u = m_graph->vertices.begin(); u != m_graph->vertices.end(); u++) {
				Structure::Vec2f resultForce(0, 0);
				for (std::vector<Structure::Vertex*>::iterator v = m_graph->vertices.begin(); v != m_graph->vertices.end(); v++) {
					if (!isAdjacent((*u)->number, (*v)->number) || not_adjacent_repel)
						resultForce = resultForce + repelForce(*u, *v);
				}
				for (std::vector<Structure::Vertex*>::iterator v = m_graph->adjList[(*u)->number].begin();
					v != m_graph->adjList[(*u)->number].end(); v++) {
					//resultForce = resultForce + attractForce(*u, *v);
				}
				(*u)->coord = (*u)->coord + resultForce;
			}
			/*for (std::vector<Structure::Vertex*>::iterator u = m_graph->vertices.begin(); u != m_graph->vertices.end(); u++) {
				(*u)->coord = (*u)->coord + (*u)->resultForce * m_c4;
			}*/
		}

	};

};

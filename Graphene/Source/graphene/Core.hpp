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
			for (int i = 0; i < m_graph->totalVertex; i++) {
				std::cout << "Vertex[ " << i << " ] position\t" << m_graph->verticies[i].coord << '\n';
			}
			std::cout << "----------------------------------\n";
		}

		const double m_c1 = 2;
		const double m_c2 = 1;
		const double m_c3 = 1;
		const double m_c4 = 0.1;

		double distance(Structure::Vertex& u, Structure::Vertex& v) {
			double dx = u.coord.x - v.coord.x;
			double dy = u.coord.y - v.coord.y;
			return sqrt(dx * dx + dy * dy);
		}

		double length(Structure::NormCoord a) {
			return sqrt(a.x * a.x + a.y * a.y);
		}

		Structure::NormCoord normalized(Structure::NormCoord a) {
			double len = length(a);
			if (len == 0) 
				return a;
			return Structure::NormCoord(a.x / len, a.y / len);
		}

		Structure::NormCoord attractForce(Structure::Vertex& u, Structure::Vertex& v) {
			double dis = distance(u, v);
			if (dis == 0.0) 
				return Structure::NormCoord(0, 0);
			double coeff = m_c1 * log10(dis / m_c2);
			return normalized((v.coord - u.coord) * coeff);
		}

		Structure::NormCoord repelForce(Structure::Vertex& u, Structure::Vertex& v) {
			double dis = distance(u, v);
			if (dis == 0.0) 
				return Structure::NormCoord(0, 0);
			double coeff = -m_c3 / sqrt(dis);
			return normalized((v.coord - u.coord) * coeff);
		}

		void initializePos() {
			for (int i = 0; i < (int)m_graph->verticies.size(); i++) {
				m_graph->verticies[i].coord = Structure::NormCoord(genRandom(), genRandom());
			}
		}

		void updatePos() {
			for (std::vector<Structure::Vertex>::iterator u = m_graph->verticies.begin(); u != m_graph->verticies.end(); u++) {
				Structure::NormCoord resultForce(0, 0);
				for (std::vector<Structure::Vertex>::iterator v = m_graph->verticies.begin(); v != m_graph->verticies.end(); v++) {
					resultForce = resultForce + attractForce(*u, *v);
				}
				for (std::vector<Structure::Vertex>::iterator v = m_graph->structure[u->number].begin(); v != m_graph->structure[u->number].end(); v++) {
					resultForce = resultForce + repelForce(*u, *v);
				}
				u->coord = u->coord + resultForce * m_c4;
				u->coord.normalize();
			}
			//for (vector<Vertex>::iterator u = graphene->verticies.begin(); u != graphene->verticies.end(); u++) {
			//	u->coord.normalize();
			//}
		}

	};

};

#pragma once
#include <string>
#include <vector>
#include <random>
#define PI 3.14159265

namespace Graphene {

	class Graph {
	public:
		struct NormCoord {
			double x;
			double y;
			NormCoord(double _x, double _y) {
				x = _x;
				y = _y;
			}
		};

		struct Vertex {

			std::string name = "";
			bool nameVisible = false;
			std::string value = "";
			bool valueVisible = false;

			Vertex() {
				static ::std::random_device rd;
				static ::std::mt19937 gen(rd());
				static ::std::uniform_real_distribution<> dis(0.0, 1.0);
				NormCoord coord = NormCoord(dis(gen), dis(gen));
			}

		};

		struct Edge {
			std::string name = "";
			bool nameVisible = false;
			std::string value = "";
			bool valueVisible = false;
			Vertex* startingVertex = nullptr;
			Vertex* endingVertex = nullptr;
			bool directed = false;

			Edge(Vertex* _startingVertex, Vertex* _endingVertex, bool _directed) {
				startingVertex = _startingVertex;
				endingVertex = _endingVertex;
				directed = _directed;

			}
		};

		std::vector<Vertex> verticies;
		std::vector<Edge> edges;


		// Handles the structure of a graph, and support for multiple graphs, graph stepping, highlighting, etc.

	};
}
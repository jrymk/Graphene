#pragma once
#include <vector>
#include "Structure.hpp"

namespace Graphene {

	class Graph {
	public:

		int totalVertex, totalEdge;
		std::vector<Structure::Vertex> verticies;
		std::vector<Structure::Edge> allEdges;
		std::vector<std::vector<Structure::Vertex> > structure;

		Graph() = default;


		void initialize(int _totalVertex, int _totalEdge, std::vector<std::pair<std::pair<int, int>, bool> >& inputEdges) {
			this->totalVertex = _totalVertex;
			this->totalEdge = _totalEdge;

			this->verticies.clear();
			this->allEdges.clear();
			this->structure.resize(this->totalVertex);

			for (int i = 0; i < this->totalVertex; i++) {
				this->verticies.emplace_back(Structure::Vertex(i));
			}

			for (std::vector<std::pair<std::pair<int, int>, bool> >::iterator it = inputEdges.begin(); it != inputEdges.end(); it++) {
				int start = it->first.first, end = it->first.second;
				bool directed = it->second;
				Structure::Edge edge = Structure::Edge(&this->verticies[start], &this->verticies[end], directed);

				this->allEdges.emplace_back(edge);
				this->structure[start].emplace_back(this->verticies[end]);
			}
		}


	};

};
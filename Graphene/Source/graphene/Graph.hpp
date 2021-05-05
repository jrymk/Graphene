#pragma once
#include <vector>
#include "Structure.hpp"

namespace Graphene {

	class Graph {
	public:

		// total number of vertices and edges
		int vertexCount = 0, edgeCount = 0;
		// list of properties of all vertices, for instance their ids and positions
		std::vector<Vertex*> vertices;
		// list of properties of all edges, for instance their connection vertices and draw thickness
		std::vector<Edge> edges;
		// the adjacency list for the structure of the graph, nothing else
		std::vector<std::vector<Vertex*>> adjList;

		Graph() = default;


		/*void importAdjList(std::vector<std::vector<int>>& inputList) {
			this->vertexCount = inputList.size();
			this->adjList.resize(vertexCount);

			for (std::vector<std::vector<int>>::iterator va = inputList.begin(); va != inputList.end(); va++) {
				for (std::vector<int>::iterator vb = va->begin(); vb != va->end(); vb++) {
					int start = it->first.first, end = it->first.second;
					bool directed = it->second;
					Edge edge = Edge(&this->vertices[start], &this->vertices[end], directed);

					this->edges.emplace_back(edge);
					this->adjList[start].emplace_back(this->vertices[end]);
				}
			}
		}*/


		void importEdges(int vertexCount, std::vector<std::pair<std::pair<int, int>, bool>>& inputEdges) {
			this->vertexCount = vertexCount;
			this->edgeCount = inputEdges.size();

			for (auto it = vertices.begin(); it != vertices.end(); it++)
				delete *it;
			this->vertices.clear();

			this->edges.clear();
			this->adjList.resize(this->vertexCount);

			for (int i = 0; i < this->vertexCount; i++) {
				this->vertices.emplace_back(new Vertex(i));
			}
	
			for (std::vector<std::pair<std::pair<int, int>, bool>>::iterator it = inputEdges.begin(); it != inputEdges.end(); it++) {
				int start = it->first.first, end = it->first.second;
				bool directed = it->second;

				this->edges.emplace_back(Edge(this->vertices[start], this->vertices[end], directed));
				this->adjList[start].emplace_back(this->vertices[end]);
				this->adjList[end].emplace_back(this->vertices[start]);
			}
		}

		bool isAdjacent(int u, int v) {
			if (std::find(adjList[u].begin(), adjList[u].end(), vertices[v]) != adjList[u].end())
				return true;
			return false;
		}


	};

};
#pragma once
#include <vector>
#include "Structure.hpp"

namespace Graphene {

	class Graph {
    public:
		//                    u                              v        edge properties
		std::vector<std::pair<Vertex*, std::vector<std::pair<Vertex*, Edge>>>> graph;

		Graph() = default;

		int getVertexCount() {
			return graph.size();
		}

		Vertex* getVertexPtr(int index) {
			return graph[index].first;
		}

		void setVertexPtr(int index, Vertex* vertex) {
			graph[index].first = vertex;
		}

		// -1: delete from end
		void deleteVertex(int id) {
			if (id == -1)
				id = getVertexCount() - 1;
			delete getVertexPtr(id);
			graph.erase(graph.begin() + id);
        }

		void resizeVertices(int count) {
			for (int i = count; i < graph.size(); i++)
                delete getVertexPtr(i);

			while (graph.size() < count)
				graph.emplace_back(new Vertex(graph.size()), std::vector<std::pair<Vertex*, Edge>>());
			
			graph.resize(count);
		}

		void resetVertices() {
            for (int i = 0; i < getVertexCount(); i++) {
                delete getVertexPtr(i);
                setVertexPtr(i, new Graphene::Vertex(i));
            }
		}

		void clearAllEdges() {
            for (auto &v : graph)
                v.second.clear();
		}

		void importEdges(int vertexCount, std::vector<std::pair<int, int>>& inputEdges) {
			resizeVertices(vertexCount);

			for (auto & inputEdge : inputEdges) {
				int u = inputEdge.first, v = inputEdge.second;
                graph[u].second.emplace_back(graph[v].first, Edge());
			}
		}

		std::vector<std::pair<Vertex*, Edge>>::iterator findEdge(int u, int v) {
			for (int n = 0; n < graph[u].second.size(); n++) {
				if ((graph[u].second)[n].first == getVertexPtr(v))
					return graph[u].second.begin() + n;
			}
			return graph[u].second.end();
		}

		void addEdge(int u, int v) {
			if (u < getVertexCount() && v < getVertexCount())
				graph[u].second.emplace_back(getVertexPtr(v), Edge());
		}

		void removeEdge(int u, int v) {
            graph[u].second.erase(findEdge(u, v));
		}

		bool isAdjacent(int u, int v) {
			if (findEdge(u, v) != graph[u].second.end())
				return true;
			return false;
		}

	};

};
#pragma once
#include <vector>
#include "Structure.hpp"

namespace Graphene {

	class Graph {
    public:
		// this data structure only stores one edge per vertex pair, for consistency in graph rendering
		//                    u                              v        edge properties, including count
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

		void removeEdge(int u, int v) {
            graph[u].second.erase(findEdge(u, v));
			graph[v].second.erase(findEdge(v, u));
		}

		bool isAdjacent(int u, int v) {
			if (findEdge(u, v) != graph[u].second.end() || findEdge(v, u) != graph[v].second.end())
				return true;
			return false;
		}

		void addEdge(int u, int v) {
			if (u < getVertexCount() && v < getVertexCount() && !isAdjacent(u, v)) {
				graph[u].second.emplace_back(getVertexPtr(v), Edge());
				graph[v].second.emplace_back(getVertexPtr(u), Edge());
			}
		}

		void evalConnectedComponent_dfs(int vertex, int component) {
			getVertexPtr(vertex)->connectedComponent = component;
			for (auto & i : graph[vertex].second) {
				if (i.first->connectedComponent == -1)
					evalConnectedComponent_dfs(i.first->getNumber(), component);
			}
		}

		void evalConnectedComponent() {
			for (int i = 0; i < getVertexCount(); i++)
				graph[i].first->connectedComponent = -1;
			int component = 0;
			for (int i = 0; i < getVertexCount(); i++) {
				if (graph[i].first->connectedComponent == -1) {
					evalConnectedComponent_dfs(i, component);
					component++;
				}
			}
		}

	};

};
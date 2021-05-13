#pragma once
#include "Graph.hpp"

namespace Graphene {

	/* Utilities to iterate through graph
	 *
	 * Usage:
	 * VertexIter it(&graph);
	 * while (it.next()) {
	 *     it.v ...
	 * }
	 */

	class VertexIter {
	private:
		Graph* graph;
		int vID = 0;

	public:
		VertexIter(Graph* graph) {
			this->graph = graph;
		}

		Vertex* v = nullptr;

		bool next() {
			if (vID >= graph->getVertexCount())
				return false;

			v = graph->getVertexPtr(vID);
			vID++;
			return true;
		}
	};

	class EdgeIter {
	private:
		Graph* graph;
		int uID = 0;
		int nID = 0;

	public:
		EdgeIter(Graph* graph) {
			this->graph = graph;
		}

		Vertex* u = nullptr;
		Vertex* v = nullptr;
		Edge* edge = nullptr;

		bool next() {

			if (uID == graph->getVertexCount())
				return false;

			while (nID >= graph->graph[uID].second.size()) { // edge size
				uID++;
				nID = 0;
				if (uID == graph->getVertexCount())
					return false;
			}

			u = graph->getVertexPtr(uID);
			v = graph->graph[uID].second[nID].first;
			edge = &graph->graph[uID].second[nID].second;
			nID++;
			return true;
		}
	};

}
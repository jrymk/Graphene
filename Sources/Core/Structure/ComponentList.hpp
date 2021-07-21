#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Component.hpp>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Properties/Properties.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core::structure {
/**
 * This is the class what the Graphene core actually touches.
 * Everything is stored in Vertex and Edge pointers rather than Uuids.
 * This is for faster, more versatile, and lock-free access to vertices.
 */
class ComponentList {
	// the source where the graph is read from to convert into component form
	gfn::core::usergraph::UserGraph* usergraph;

	gfn::core::properties::Properties* props;

	std::unordered_set<Vertex*> vertices;
	std::unordered_set<Edge*> edges;
	std::unordered_set<Component*> components;

  public:
	bool bindUserGraph(gfn::core::usergraph::UserGraph* usergraph) { this->usergraph = usergraph; }

	bool bindProperties(gfn::core::properties::Properties* properties) { this->props = properties; }

	void _loadFromUserGraphDFS(Component* c, Vertex* vertex, gfn::core::Uuid uId,
							   std::unordered_map<gfn::core::Uuid, Vertex*>& pending) {
		c->getAdjList().insert(
			{vertex, std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>()});
		pending.erase(uId);
		vertex->component = c;

		auto userIt = usergraph->getAdjList().find(uId);
		for (auto& v : userIt->second) {
			if (pending.find(v.first) != pending.end()) { // still pending
				_loadFromUserGraphDFS(c, vertex, v.first, pending);
			}
		}
	}

	void loadFromUserGraph() {
		usergraph->propsCheckup(true);

		auto userAdjList = usergraph->getAdjList();

		for (auto& v : vertices)
			delete v;
		for (auto& e : edges)
			delete e;
		for (auto& c : components)
			delete c;
		vertices.clear();
		edges.clear();
		components.clear();

		for (auto& u : usergraph->getAdjList()) { // initialize new vertex and edge objects
			auto vertex = new Vertex();
			vertex->uuid = u.first;
			vertex->prop = props->getVertexProp(u.first);
			vertices.insert(vertex);
			for (auto& v : u.second) {
				for (auto& e : v.second) {
					auto edge = new Edge();
					edge->edgeUuid = e;
					edge->startVertexUuid = u.first;
					edge->endVertexUuid = v.first;
					edge->prop = props->getEdgeProp(e);
					edges.insert(edge);
				}
			}
		}

		std::unordered_map<gfn::core::Uuid, Vertex*> pendingVertices;
		std::unordered_set<Vertex*> previousComponentRoot;

		for (auto& v : vertices) {
			pendingVertices.insert({v->uuid, v});
			if (v->prop->_prevComponentRoot)
				previousComponentRoot.insert(v);
		}

		while (!pendingVertices.empty()) {
			auto component = new Component();
			components.insert(component);

			// recursively recreate vertex entries in component adjacency list (no edges)
			_loadFromUserGraphDFS(component, pendingVertices.begin()->second, pendingVertices.begin()->first,
								  pendingVertices);
		}

		/// TODO: Component root management
	}
};

} // namespace gfn::core::structure
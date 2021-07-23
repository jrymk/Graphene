#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Component/Component.hpp>
#include <Core/Structure/BlockCutTree/BlockCutTree.hpp>
#include <Core/Structure/BlockCutTree/BlockCutTreeBuilder.hpp>
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
	gfn::core::properties::Properties* props;

	std::unordered_set<Vertex*> vertices;
	std::unordered_set<Edge*> edges;
	std::unordered_set<Component*> components;

  public:
	~ComponentList() {
		for (auto& v : vertices)
			delete v;
		for (auto& e : edges)
			delete e;
		for (auto& c : components)
			delete c;
	}

	void bindProperties(gfn::core::properties::Properties* properties) { this->props = properties; }

	/// @brief Constructs all components from UserGraph (must run in sync with core cycle)
	/// @param usergraph the source graph where the components are constructed from
	void componentify(gfn::core::usergraph::UserGraph* usergraph) {
		usergraph->propsCheckup(true);

		for (auto& v : vertices)
			delete v;
		vertices.clear();
		for (auto& e : edges)
			delete e;
		edges.clear();
		for (auto& c : components)
			delete c;
		components.clear();

		// maps from uuid to vertex objects (for edge assignment)
		std::unordered_map<gfn::core::Uuid, Vertex*> mapping;
		// vertices that were the component root in the previous graph update
		std::unordered_set<Vertex*> previousComponentRoot;

		// initialize new vertex and edge objects
		for (auto& u : usergraph->getAdjList()) {
			auto vertex = new Vertex();
			vertex->uuid = u.first;
			vertex->prop = props->getVertexProp(u.first);
			vertices.insert(vertex);
			mapping.insert({vertex->uuid, vertex});
			// keep a list of previous component roots to later assign them back to the fresh component objects and
			// maximize the reusability (so component properties can be kept)
			// this is a fairly bad implementation, and might be changed in the future
			if (vertex->prop->_prevComponentRoot)
				previousComponentRoot.insert(vertex);
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

		// a set of vertices that are pending to get a component assigned
		std::unordered_set<Vertex*> pendingV(vertices.begin(), vertices.end());

		// spawn in as many component as needed
		while (!pendingV.empty()) {
			auto component = new Component();
			components.insert(component);
			// recursively recreate vertex entries in component adjacency list (no edges)
			_componentifyDfs(component, (*pendingV.begin())->uuid, mapping, pendingV, usergraph);
		}

		// construct edges for the components
		for (auto& e : edges) {
			auto u = mapping.find(e->startVertexUuid)->second;
			auto v = mapping.find(e->endVertexUuid)->second;
			if (u->component != v->component) {
				logInsert("Componentifier: Edge {") logInsert(e->edgeUuid) logInsert("} start vertex {")
					logInsert(u->uuid) logInsert("} component {") logInsert(u->component->uuid)
						logInsert("} is different from end vertex {") logInsert(v->uuid) logInsert("} component {")
							logInsert(v->component->uuid) logWarning("}, something went wrong assigning components");
			}
			auto c = u->component;
			auto uIt = c->getAdjList().find(u);
			auto vIt = c->getAdjList().find(v);
			uIt->second.first.insert(v); // the connectivity-based adj list
			vIt->second.first.insert(u);
			uIt->second.second.insert({v, std::unordered_set<Edge*>()}); // the edge-based adj list
			uIt->second.second.find(v)->second.insert(e);
		}

		/// TODO: Fix up the crappy component reuse implementation

		for (auto& v : previousComponentRoot) {
			v->component->uuid = v->prop->_prevComponent;
			logInsert("Componentifier: Assigned component uuid {") logInsert(v->prop->_prevComponent)
				logInsert("} from previous root vertex {") logInsert(v->uuid) logVerbose("}");
		}
		for (auto& c : components) {
			if (c->uuid == gfn::core::uuid::createNil()) {
				auto uuid = gfn::core::uuid::createUuid();
				c->uuid = uuid;
				c->root = c->getAdjList().begin()->first;
				logInsert("Componentifier: Assigned component uuid {") logInsert(uuid)
					logInsert("} (auto-generated) and assigned vertex {")
						logInsert(c->getAdjList().begin()->first->uuid) logVerbose("} as root");
			}
		}

		// finish up everything
		for (auto& c : components) {
			for (auto& v : c->getAdjList()) {
				v.first->prop->_prevComponentRoot = false;
				v.first->prop->_prevComponent = c->uuid;
			}
			c->root->prop->_prevComponentRoot = true;
		}
	}

	///@param c the component object for this whole dfs session
	///@param uId the uuid for the currently traversed vertex
	///@param mapping the mapping from uuid to vertex* for reading the uuid based usergraph and find the correct vertex
	/// to construct the object based component adjacency list
	///@param pending the set of vertices who does not have a component. here it is used as a "visited" tag
	void _componentifyDfs(Component* c, gfn::core::Uuid uId, std::unordered_map<gfn::core::Uuid, Vertex*>& mapping,
						  std::unordered_set<Vertex*>& pending, gfn::core::usergraph::UserGraph* usergraph) {
		Vertex* u = mapping.find(uId)->second;
		c->getAdjList().insert(
			{u, std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>()});
		pending.erase(u);
		// assign the component
		u->component = c;

		auto userIt = usergraph->getAdjList().find(uId);
		for (auto& v : userIt->second) {
			if (pending.find(mapping.find(v.first)->second) != pending.end()) { // still pending
				_componentifyDfs(c, v.first, mapping, pending, usergraph);
			}
		}
	}
};

} // namespace gfn::core::structure
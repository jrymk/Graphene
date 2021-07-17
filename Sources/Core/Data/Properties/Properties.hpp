#pragma once

#include <unordered_map>
#include <System/Random/Random.hpp>
#include <Core/Data/Properties/Uuid.hpp>
#include <Core/Data/Properties/Vertex.hpp>
#include <Core/Data/Properties/Edge.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core::data {
// all the vertices, edges, components store their information here
// including position, color, except how vertices are connected together.
// accessed through uuid, it will be compatible with the "command" topology
class Properties {
  private:
	std::unordered_map<gfn::core::Uuid, gfn::core::data::VertexProp*> vertexProps;
	std::unordered_map<gfn::core::Uuid, gfn::core::data::EdgeProp*> edgeProps;

  public:
	// every new vertex should start from here.
	// make sure to have the properties structure bound to the vertex before putting it in the graph structure
	gfn::core::Uuid allocateVertex() {
		while (true) {
			gfn::core::Uuid uuid = gfn::system::uuid::createUuid();
			if (vertexProps.find(uuid) != vertexProps.end()) {
				logInsert("Vertex UUID: ");
				logInsert(uuid);
				logWarning(" creation collided! How is this even possible? (retrying)");
			} else {
				auto prop = new gfn::core::data::VertexProp();
				prop->uuid = uuid;
				vertexProps.insert(std::make_pair(uuid, prop));
				return uuid;
			}
		}
	}

	// make sure to have the properties structure bound to the edge before putting it in the graph structure
	gfn::core::Uuid allocateEdge() {
		while (true) {
			gfn::core::Uuid uuid = gfn::system::uuid::createUuid();
			if (edgeProps.find(uuid) != edgeProps.end()) {
				logInsert("Edge UUID: ");
				logInsert(uuid);
				logWarning(" creation collided! How is this even possible? (retrying)");
			} else {
				auto prop = new gfn::core::data::EdgeProp();
				prop->uuid = uuid;
				edgeProps.insert(std::make_pair(uuid, prop));
				return uuid;
			}
		}
	}

	bool vertexExist(gfn::core::Uuid uuid, bool expected = false) {
		bool exist = vertexProps.find(uuid) != vertexProps.end();
		if (expected && !exist) {
			logInsert("Vertex UUID: ");
			logInsert(uuid);
			logWarning(" unexpectedly does not exist");
		}
		return (vertexProps.find(uuid) != vertexProps.end());
	}

	bool edgeExist(gfn::core::Uuid uuid, bool expected = false) {
		bool exist = edgeProps.find(uuid) != edgeProps.end();
		if (expected && !exist) {
			logInsert("Edge UUID: ");
			logInsert(uuid);
			logWarning(" unexpectedly does not exist");
		}
		return (edgeProps.find(uuid) != edgeProps.end());
	}

	gfn::core::data::VertexProp* getVertexProp(gfn::core::Uuid uuid) {
		auto it = vertexProps.find(uuid);
		if (it == vertexProps.end()) {
			logInsert("Vertex UUID: Attempt to get properties for vertex ");
			logInsert(uuid);
			logWarning(" failed");
			return nullptr;
		} else
			return it->second;
	}

	gfn::core::data::EdgeProp* getEdgeProp(gfn::core::Uuid uuid) {
		auto it = edgeProps.find(uuid);
		if (it == edgeProps.end()) {
			logInsert("Edge UUID: Attempt to get properties for edge ");
			logInsert(uuid);
			logWarning(" failed");
			return nullptr;
		} else
			return it->second;
	}

	bool eraseVertex(gfn::core::Uuid uuid) {
		auto prop = getVertexProp(uuid);
		if (prop != nullptr) {
			delete prop;
			vertexProps.erase(uuid);
			return true;
		} else {
			logInsert("Vertex UUID: Attempt to delete vertex ");
			logInsert(uuid);
			logWarning(" failed");
			return false;
		}
	}

	bool eraseEdge(gfn::core::Uuid uuid) {
		auto prop = getEdgeProp(uuid);
		if (prop != nullptr) {
			delete prop;
			edgeProps.erase(uuid);
			return true;
		} else {
			logInsert("Edge UUID: Attempt to delete edge ");
			logInsert(uuid);
			logWarning(" failed");
			return false;
		}
	}
};
} // namespace gfn::core::data
#pragma once

#include <unordered_map>
#include <System/Random/Random.hpp>
#include <Core/Data/Uuid.hpp>
#include <Core/Data/Properties/Vertex.hpp>
#include <Core/Data/Properties/Edge.hpp>
#include <Core/Data/Properties/Component.hpp>
#include <Core/Data/Properties/BiconnectedComponent.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core::data {
// all the vertices, edges, components store their information here
// including position, color, except how vertices are connected together.
// accessed through uuid, it will be compatible with the "command" topology
class Properties {
  private:
	std::unordered_map<gfn::core::Uuid, gfn::core::data::VertexProp> vertexProps;
	std::unordered_map<gfn::core::Uuid, gfn::core::data::EdgeProp> edgeProps;
	std::unordered_map<gfn::core::Uuid, gfn::core::data::ComponentProp> componentProps;
	std::unordered_map<gfn::core::Uuid, gfn::core::data::BiconnectedComponentProp> biconnectedComponentProps;

  public:
	void newVertexProp(gfn::core::Uuid uuid) {
		vertexProps.insert(std::make_pair(uuid, gfn::core::data::VertexProp(uuid)));
	}

	void newEdgeProp(gfn::core::Uuid uuid) { edgeProps.insert(std::make_pair(uuid, gfn::core::data::EdgeProp(uuid))); }

	void newComponentProp(gfn::core::Uuid uuid) {
		componentProps.insert(std::make_pair(uuid, gfn::core::data::ComponentProp(uuid)));
	}

	void newBiconnectedComponentProp(gfn::core::Uuid uuid, gfn::core::data::BiconnectedComponentProp::Type type) {
		biconnectedComponentProps.insert(std::make_pair(uuid, gfn::core::data::BiconnectedComponentProp(uuid, type)));
	}

	gfn::core::data::VertexProp& getVertexProp(gfn::core::Uuid uuid) {
		auto it = vertexProps.find(uuid);
		if (it == vertexProps.end()) {
			logInsert("Vertex UUID: Attempt to get properties for vertex ");
			logInsert(uuid);
			logWarning(" failed");
		}
		return it->second;
	}

	gfn::core::data::EdgeProp& getEdgeProp(gfn::core::Uuid uuid) {
		auto it = edgeProps.find(uuid);
		if (it == edgeProps.end()) {
			logInsert("Edge UUID: Attempt to get properties for edge ");
			logInsert(uuid);
			logWarning(" failed");
		}
		return it->second;
	}

	gfn::core::data::ComponentProp& getComponentProp(gfn::core::Uuid uuid) {
		auto it = componentProps.find(uuid);
		if (it == componentProps.end()) {
			logInsert("Component UUID: Attempt to get properties for component ");
			logInsert(uuid);
			logWarning(" failed");
		}
		return it->second;
	}

	gfn::core::data::BiconnectedComponentProp& getBiconnectedComponentProp(gfn::core::Uuid uuid) {
		auto it = biconnectedComponentProps.find(uuid);
		if (it == biconnectedComponentProps.end()) {
			logInsert("Component UUID: Attempt to get properties for component ");
			logInsert(uuid);
			logWarning(" failed");
		}
		return it->second;
	}

	void removeVertexProp(gfn::core::Uuid uuid) { vertexProps.erase(uuid); }

	void removeEdgeProp(gfn::core::Uuid uuid) { edgeProps.erase(uuid); }

	void removeComponentProp(gfn::core::Uuid uuid) { componentProps.erase(uuid); }

	void removeBiconnectedComponentProp(gfn::core::Uuid uuid) { biconnectedComponentProps.erase(uuid); }
};
} // namespace gfn::core::data
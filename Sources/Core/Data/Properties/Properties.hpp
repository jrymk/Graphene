#pragma once

#include <unordered_map>
#include <System/Random/Random.hpp>
#include <Core/Data/Uuid.hpp>
#include <Core/Data/Properties/Vertex.hpp>
#include <Core/Data/Properties/Edge.hpp>
#include <Core/Data/Properties/Component.hpp>
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

  public:
	bool newVertexProp(gfn::core::Uuid uuid) {
		vertexProps.insert(std::make_pair(uuid, gfn::core::data::VertexProp(uuid)));
	}

	bool newEdgeProp(gfn::core::Uuid uuid) { edgeProps.insert(std::make_pair(uuid, gfn::core::data::EdgeProp(uuid))); }

	bool newComponentProp(gfn::core::Uuid uuid) {
		componentProps.insert(std::make_pair(uuid, gfn::core::data::ComponentProp(uuid)));
	}

	gfn::core::data::VertexProp& getVertexProp(gfn::core::Uuid uuid) {
		auto it = vertexProps.find(uuid);
		if (it == vertexProps.end()) {
			logInsert("Vertex UUID: Attempt to get properties for vertex ");
			logInsert(uuid);
			logWarning(" failed");
			gfn::core::data::VertexProp tp("");
			return tp;
		} else
			return it->second;
	}

	gfn::core::data::EdgeProp& getEdgeProp(gfn::core::Uuid uuid) {
		auto it = edgeProps.find(uuid);
		if (it == edgeProps.end()) {
			logInsert("Edge UUID: Attempt to get properties for edge ");
			logInsert(uuid);
			logWarning(" failed");
			gfn::core::data::EdgeProp tp("");
			return tp;
		} else
			return it->second;
	}

	gfn::core::data::ComponentProp& getComponentProp(gfn::core::Uuid uuid) {
		auto it = componentProps.find(uuid);
		if (it == componentProps.end()) {
			logInsert("Component UUID: Attempt to get properties for component ");
			logInsert(uuid);
			logWarning(" failed");
			gfn::core::data::ComponentProp tp("");
			return tp;
		} else
			return it->second;
	}

	bool removeVertexProp(gfn::core::Uuid uuid) { vertexProps.erase(uuid); }

	bool removeEdgeProp(gfn::core::Uuid uuid) { edgeProps.erase(uuid); }

	bool removeComponentProp(gfn::core::Uuid uuid) { componentProps.erase(uuid); }
};
} // namespace gfn::core::data
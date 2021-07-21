#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Properties/Properties.hpp>

namespace gfn::core::structure {
/// @brief The Component class is the biggest unit for graph calculations, stores both directional-edge-based adj list
/// (same as usergraph) and connected-based adj list
class Component {
	std::unordered_map<Vertex*,
					   std::pair<std::unordered_set<Vertex*>,							// connected-based
								 std::unordered_map<Vertex*, std::unordered_set<Edge*>> // directional-edge-based
								 >>
		adjList;

  public:
	gfn::core::Uuid componentUuid;

	Vertex* root;

	Component() { componentUuid = gfn::core::uuid::createNil(); }

	std::unordered_map<Vertex*,
					   std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>>&
	getAdjList() {
		return adjList;
	}
};
} // namespace gfn::core::structure
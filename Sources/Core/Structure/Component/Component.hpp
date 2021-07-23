#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Structure/BlockCutTree/BlockCutTree.hpp>
#include <Core/Properties/Properties.hpp>

namespace gfn::core::structure {
/// @brief The Component class is the biggest unit for graph calculations, stores both directional-edge-based adj list
/// (same as usergraph) and connectivity-based adj list
class Component {
  private:
	std::unordered_map<Vertex*,
					   std::pair<std::unordered_set<Vertex*>,							// connectivity-based
								 std::unordered_map<Vertex*, std::unordered_set<Edge*>> // directional-edge-based
								 >>
		adjList;
	gfn::core::structure::BlockCutTree blockCutTree;

  public:
	gfn::core::Uuid uuid;

	Vertex* root;

	Component() { uuid = gfn::core::uuid::createNil(); }

	std::unordered_map<Vertex*,
					   std::pair<std::unordered_set<Vertex*>, std::unordered_map<Vertex*, std::unordered_set<Edge*>>>>&
	getAdjList() {
		return adjList;
	}

	/// @brief get all vertices that are adjacent to the given vertex the component (non directional)
	/// @returns a set of all the adjacent vertices
	std::unordered_set<Vertex*> getAdjacentVertices(Vertex* v) {
		auto vIt = adjList.find(v);
		if (vIt == adjList.end()) {
			logInsert("Component: Request to get adjacent vertices for vertex {") logInsert(v->uuid)
				logInsert("} in component {") logInsert(uuid)
					logWarning("} failed (vertex does not exist in component)");
			return std::unordered_set<Vertex*>();
		}
		return vIt->second.first;
	}

	/// @brief check if vertices are adjacent to each other in the component (non directional)
	bool isAdjacent(Vertex* u, Vertex* v) {
		auto uIt = adjList.find(u);
		if (uIt == adjList.end()) {
			logInsert("Component: Request to check adjacentivity for vertex {") logInsert(u->uuid) logInsert("} and {")
				logInsert(v->uuid) logInsert("} in component {") logInsert(uuid)
					logWarning("} failed (start vertex does not exist in component)");
			return false;
		}
		return uIt->second.first.find(v) != uIt->second.first.end();
	}

	gfn::core::structure::BlockCutTree& getBlockCutTree() { return blockCutTree; }
};
} // namespace gfn::core::structure
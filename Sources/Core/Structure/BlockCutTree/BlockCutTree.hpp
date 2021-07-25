#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Structure/BlockCutTree/BiconnectedComponent.hpp>
#include <Core/Properties/Properties.hpp>

namespace gfn::core::structure {
// each component owns one block cut tree, rebuilt upon graph update (componentifier)
class BlockCutTree {
  public:
	// the block cut tree in adjacency list representation
	std::unordered_map<BiconnectedComponent*, std::unordered_set<BiconnectedComponent*>> adjList;
	// the biconnected component a vertex is in
	std::unordered_map<Vertex*, BiconnectedComponent*> vertexBcc;

	///@brief delete old biconnected component objects and clear adj list
	void reset() {
		// free up biconnected component objects memory
		for (auto& i : adjList)
			delete i.first;
		adjList.clear();
	}

	~BlockCutTree() { reset(); }

	bool addEdge(BiconnectedComponent* u, BiconnectedComponent* v, bool returnFalseOnAlreadyExist = false) {
		auto uIt = adjList.find(u);
		auto vIt = adjList.find(v);
		if (uIt == adjList.end()) {
			// logBuffer->warning("BlockCutTree: Add edge operation failed (bcc u does not exist)", LOGINFO);
			return false;
		}
		if (vIt == adjList.end()) {
			// logBuffer->warning("BlockCutTree: Add edge operation failed (bcc v does not exist)", LOGINFO);
			return false;
		}
		if (u->getType() == v->getType()) {
			// logBuffer->warning("BlockCutTree: Add edge operation failed between {", LOGINFO);
			// logBuffer->join(u->uuid);
			// logBuffer->join("} and {");
			// logBuffer->join(v->uuid);
			// logBuffer->join("} (adding edge between biconnected component with same type", LOGINFO);
			return false;
		}
		if (!uIt->second.insert(v).second) {
			// logBuffer->warning("BlockCutTree: Edge not added from {", LOGINFO);
			// logBuffer->join(u->uuid);
			// logBuffer->join("} to {");
			// logBuffer->join(v->uuid);
			// logBuffer->join("} (already exists");
			if (returnFalseOnAlreadyExist)
				return false;
		}
		if (!vIt->second.insert(u).second) {
			// logBuffer->warning("BlockCutTree: Edge not added from {", LOGINFO);
			// logBuffer->join(v->uuid);
			// logBuffer->join("} to {");
			// logBuffer->join(u->uuid);
			// logBuffer->join("} (already exists");
			if (returnFalseOnAlreadyExist)
				return false;
		}
		return true;
	}
};
} // namespace gfn::core::structure
#pragma once

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <Core/Data/Uuid.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Tasks/Tasks.hpp>

namespace gfn::core::data {
// maintains the structure of the graph
class AdjacencyList {
  private:
	gfn::core::TaskManager* taskManager;

	std::unordered_map<gfn::core::Uuid, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>>
		adjList;

  public:
	void bindTaskManager(gfn::core::TaskManager* taskManager) { this->taskManager = taskManager; }

	std::unordered_map<gfn::core::Uuid, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>>&
	getRawList() {
		return adjList;
	}

	void addVertex(gfn::core::Uuid vID) {
		adjList.insert(std::make_pair(vID, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>()));

		logInsert("core::data: Add vertex ");
		logVerbose(vID);
	}

	bool removeVertex(gfn::core::Uuid vID) {
		auto it = adjList.find(vID);
		if (it == adjList.end()) {
			logInsert("core::data: Remove vertex ");
			logInsert(vID);
			logWarning(" failed: does not exist");
			return false;
		}
		adjList.erase(it);
		logInsert("core::data: Remove vertex ");
		logVerbose(vID);
	}

	bool addEdge(gfn::core::Uuid v1ID, gfn::core::Uuid v2ID, gfn::core::Uuid edgeID) {
		auto v1it = adjList.find(v1ID);
		if (v1it == adjList.end()) {
			logInsert("core::data: Add edge with v1: ");
			logInsert(v1ID);
			logWarning(" failed: does not exist");
			return false;
		}

		auto v2it = adjList.find(v2ID);
		if (v2it == adjList.end()) {
			logInsert("core::data: Add edge with v2: ");
			logInsert(v2ID);
			logWarning(" failed: does not exist");
			return false;
		}

		auto vEit = v1it->second.find(v2ID);
		if (vEit == v1it->second.end())
			v1it->second.insert(std::make_pair(v2ID, std::unordered_set<gfn::core::Uuid>()));

		vEit->second.insert(edgeID);

		logInsert("core::data: Add edge from ");
		logInsert(v1ID);
		logInsert(" to ");
		logInsert(v2ID);
		logInsert(" edge ");
		logVerbose(edgeID);

		return true;
	}

	bool removeEdge(gfn::core::Uuid v1ID, gfn::core::Uuid v2ID, gfn::core::Uuid edgeID) {
		auto v1it = adjList.find(v1ID);
		if (v1it == adjList.end()) {
			logInsert("core::data: Remove edge with v1: ");
			logInsert(v1ID);
			logWarning(" failed: does not exist");
			return false;
		}

		auto vEit = v1it->second.find(v2ID);
		if (vEit == v1it->second.end()) {
			logInsert("core::data: Remove edge from ");
			logInsert(v1ID);
			logInsert(" to ");
			logInsert(v2ID);
			logWarning(" failed: edge from v1 to v2 does not exist");
			return false;
		}

		auto eIt = vEit->second.find(edgeID);
		if (eIt == vEit->second.end()) {
			logInsert("core::data: Remove edge from ");
			logInsert(v1ID);
			logInsert(" to ");
			logInsert(v2ID);
			logInsert(" edge ");
			logInsert(edgeID);
			logWarning(" failed: edge from v1 to v2 with edge id does not exist");
			return false;
		}

		vEit->second.erase(eIt);

		logInsert("core::data: Remove edge from ");
		logInsert(v1ID);
		logInsert(" to ");
		logInsert(v2ID);
		logInsert(" edge ");
		logVerbose(edgeID);

		if (vEit->second.empty()) {
			v1it->second.erase(vEit);
			logVerbose("core::data: Removed empty v2 entry from v1");
		}

		return true;
	}

	bool removeAllEdges(gfn::core::Uuid v1ID, gfn::core::Uuid v2ID) {
		auto v1it = adjList.find(v1ID);
		if (v1it == adjList.end()) {
			logInsert("core::data: Remove edge with v1: ");
			logInsert(v1ID);
			logWarning(" failed: does not exist");
			return false;
		}

		auto vEit = v1it->second.find(v2ID);
		if (vEit == v1it->second.end()) {
			logInsert("core::data: Remove edge from ");
			logInsert(v1ID);
			logInsert(" to ");
			logInsert(v2ID);
			logWarning(" failed: edge from v1 to v2 does not exist");
			return false;
		}

		v1it->second.erase(vEit);

		logInsert("core::data: Remove all edges from ");
		logInsert(v1ID);
		logInsert(" to ");
		logVerbose(v2ID);

		return true;
	}
};

}; // namespace gfn::core::data
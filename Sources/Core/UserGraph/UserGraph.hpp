#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/Properties.hpp>

///@brief usergraph is the place where the master graph structure is stored, stored in UUIDs, it is friendly to string
/// command inputs, have more tolerance to misuse.
namespace gfn::core::usergraph {
class UserGraph {
	std::unordered_map<gfn::core::Uuid, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>>
		adjList;

	gfn::core::properties::Properties* props;

  public:
	UserGraph(gfn::core::properties::Properties* propsObject) : props(propsObject) {}

	// add a new vertex with auto-generated uuid
	/// @returns  a pair with first indicating if it was successful and second the uuid auto-generated that represents
	/// the vertex created
	std::pair<bool, gfn::core::Uuid> addVertex() {
		for (int retries = 1; retries <= 5; retries++) {
			auto uuid = gfn::core::uuid::createUuid();
			if (adjList.insert({uuid, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>()})
					.second) {
				// add entry to adjacency list
				props->newVertexProp(uuid);
				logInsert("UserGraph: Added new vertex [") logInsert(uuid) logVerbose("] (auto-generated)");
				return {true, uuid};
			}
			// prop already existed somehow
			logInsert("UserGraph: Add new vertex [") logInsert(uuid)
				logInsert("] (auto-generated) failed (already exists, retrying: ") logInsert(std::to_string(retries))
					logWarning("/5)");
		}
		logError("UserGraph: (unexpected uuid creation failure)");
		return {false, gfn::core::uuid::createNil()};
	}

	// add a new vertex with the given uuid
	bool addVertex(gfn::core::Uuid uuid) {
		if (adjList.insert({uuid, std::unordered_map<gfn::core::Uuid, std::unordered_set<gfn::core::Uuid>>()}).second) {
			// add entry to adjacency list
			props->newVertexProp(uuid);
			logInsert("UserGraph: Added new vertex [") logInsert(uuid) logVerbose("]");
			return true;
		}
		logInsert("UserGraph: Add new vertex [") logInsert(uuid) logError("] failed (uuid given already exists)");
		return false;
	}

	///@brief Deletes vertex with the given uuid. erase**Properties will also clear the properties tied to the uuid,
	/// without erase so you can create a new vertex/edge with the same uuid to retreive all the properties.
	/// removeAllAdjacentEdges remove edges that points to the vertex getting deleted
	/// @returns if the deleting was successful
	bool removeVertex(gfn::core::Uuid uuid, bool eraseVertexProperties = true, bool eraseEdgeProperties = true,
					  bool removeAllAdjacentEdges = true) {
		auto uIt = adjList.find(uuid);
		if (uIt == adjList.end()) {
			logInsert("UserGraph: Remove vertex [") logInsert(uuid) logWarning("] failed (not found)");
			return false;
		}

		logInsert("UserGraph: Removing edges adjacent to vertex [") logInsert(uuid)
			logInsert("] with eraseEdgeProperties ") logVerbose(eraseEdgeProperties ? "enabled" : "disabled");
		if (eraseEdgeProperties) {
			for (auto& v : uIt->second) {
				logInsert("    vertex [") logInsert(v.first) logVerbose("]");
				for (auto& e : v.second) {
					logInsert("        edge [") logInsert(e) logVerbose("]");
					/// TODO: erase edge property
				}
			}
		}

		if (removeAllAdjacentEdges) {
			logVerbose("  <edges TO the vertex>");
			for (auto& u : adjList) {
				if (u.first != uuid) {
					for (auto& v : u.second) {
						if (v.first == uuid) {
							logInsert("    from vertex [") logInsert(u.first) logVerbose("]");

							if (eraseEdgeProperties) {
								for (auto& e : v.second) {
									logInsert("        edge [") logInsert(e) logVerbose("]");
									/// TODO: erase edge property
								}
							}

							u.second.erase(uuid); // erase vertex-edge entries from other vertices
						}
					}
				}
			}
		}

		logInsert("UserGraph: Removing vertex [") logInsert(uuid) logInsert("] with eraseVertexProperties ")
			logVerbose(eraseVertexProperties ? "enabled" : "disabled");
		if (eraseVertexProperties)
			props->eraseVertexProp(uuid);

		adjList.erase(uuid);
	}

	// add a new edge with auto-generated uuid between two given vertices
	/// @returns a pair with first indicating if it was successful and second the uuid auto-generated that represents
	/// the edge created
	std::pair<bool, gfn::core::Uuid> addEdge(gfn::core::Uuid startVertex, gfn::core::Uuid endVertex) {
		auto startIt = adjList.find(startVertex);
		if (startIt == adjList.end()) {
			logInsert("UserGraph: Add edge failed (startVertex [") logInsert(startVertex) logInsert("] does not exist");
			return {false, gfn::core::uuid::createNil()};
		}

		auto endIt = adjList.find(endVertex);
		if (endIt == adjList.end()) {
			logInsert("UserGraph: Add edge failed (endVertex [") logInsert(endVertex) logInsert("] does not exist");
			return {false, gfn::core::uuid::createNil()};
		}

		for (int retries = 1; retries <= 5; retries++) {
			auto edgeId = gfn::core::uuid::createUuid();
			if (props->getEdgeProp(edgeId)) { // prop does not exist
				// add entry to adjacency list
				if (startIt->second.insert({endVertex, std::unordered_set<gfn::core::Uuid>()}).second) {
					// created new endVertex entry
				}

				if (startIt->second.find(endVertex)->second.insert(edgeId).second) {
					logInsert("UserGraph: Added new edge with startVertex [") logInsert(startVertex)
						logInsert("] endVertex [") logInsert(endVertex) logInsert("] edge [") logInsert(edgeId)
							logVerbose("] successfully");

					props->newEdgeProp(edgeId);
					props->getEdgeProp(edgeId)->startVertexUuid = startVertex;
					props->getEdgeProp(edgeId)->edgeUuid = edgeId;
					return {true, edgeId};
				}
				logInsert("UserGraph: Add new edge with startVertex [") logInsert(startVertex)
					logInsert("] endVertex [") logInsert(endVertex) logInsert("] edge [") logInsert(edgeId)
						logError("] failed (unexpected insertion failure)");
				return {false, gfn::core::uuid::createNil()};
			}
			// prop already existed somehow
			logInsert("UserGraph: Add new edge with startVertex [") logInsert(startVertex) logInsert("] endVertex [")
				logInsert(endVertex) logInsert("] failed (edge [") logInsert(edgeId)
					logInsert("] (auto-generated) already exists, retrying: ") logInsert(std::to_string(retries))
						logWarning("/5)");
		}
		logError("UserGraph: (unexpected uuid creation failure)");
		return {false, gfn::core::uuid::createNil()};
	}

	// add a new edge with the given uuid between two given vertices
	bool addEdge(gfn::core::Uuid startVertex, gfn::core::Uuid endVertex, gfn::core::Uuid edgeUuid) {
		auto startIt = adjList.find(startVertex);
		if (startIt == adjList.end()) {
			logInsert("UserGraph: Add edge failed (startVertex [") logInsert(startVertex) logInsert("] does not exist");
			return false;
		}

		auto endIt = adjList.find(endVertex);
		if (endIt == adjList.end()) {
			logInsert("UserGraph: Add edge failed (endVertex [") logInsert(endVertex) logInsert("] does not exist");
			return false;
		}

		if (props->getEdgeProp(edgeUuid)) { // prop does not exist
			// add entry to adjacency list
			if (startIt->second.insert({endVertex, std::unordered_set<gfn::core::Uuid>()}).second) {
				// created new endVertex entry
			}

			if (startIt->second.find(endVertex)->second.insert(edgeUuid).second) {
				logInsert("UserGraph: Added new edge with startVertex [") logInsert(startVertex)
					logInsert("] endVertex [") logInsert(endVertex) logInsert("] edge [") logInsert(edgeUuid)
						logVerbose("] successfully");

				props->newEdgeProp(edgeUuid);
				props->getEdgeProp(edgeUuid)->startVertexUuid = startVertex;
				props->getEdgeProp(edgeUuid)->edgeUuid = edgeUuid;
				return true;
			}

			logInsert("UserGraph: Add new edge with startVertex [") logInsert(startVertex) logInsert("] endVertex [")
				logInsert(endVertex) logInsert("] edge [") logInsert(edgeUuid)
					logError("] failed (unexpected insertion failure)");
			return false;
		}
		logInsert("UserGraph: Add new edge with startVertex [") logInsert(startVertex) logInsert("] endVertex [")
			logInsert(endVertex) logInsert("] edge [") logInsert(edgeUuid)
				logWarning("] failed (edge uuid given already exists)");
		return false;
	}

	///@brief remove edge with specified uuid (one edge)
	///@returns if the edge deleting is successful or not (edge not found). If eraseProperties is on, false will also be
	/// returned if properties not found (properties delete unsuccessfullk)
	bool removeEdge(gfn::core::Uuid edgeUuid, bool eraseProperties) {
		auto prop = props->getEdgeProp(edgeUuid);
		if (prop) { // prop exists
					// the "use start and end vertices from prop" route
			auto startIt = adjList.find(prop->startVertexUuid);
			if (startIt != adjList.end()) {
				auto endIt = startIt->second.find(prop->endVertexUuid);
				if (endIt != startIt->second.end()) {
					if (endIt->second.erase(edgeUuid)) {
						// erased the edge successfully
						if (endIt->second.empty()) {
							// no more edges between the given start-end vertex pair
							startIt->second.erase(endIt);
							if (!eraseProperties || (eraseProperties && props->eraseEdgeProp(edgeUuid)))
								return true;
							logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
								logWarning("] failed (edge prop deletion failed)");
							return false;
						}
						// still have edges between the given start-end vertex pair
						return true;
					}
					// can find start and end vertex entries but edge deletion failed, entering full scan route
					logInsert("UserGraph: Remove edge [") logInsert(edgeUuid) logWarning(
						"] failed (vertices from prop found but not the edge, trying full scan from usergraph)");
				}
				logInsert("UserGraph: Remove edge [") logInsert(edgeUuid) logWarning(
					"] failed (endVertex from prop entry not found from startVertex, trying full scan from usergraph)");
			}
			logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
				logWarning("] failed (startVertex from prop not found, trying full scan from usergraph)");
		} else {
			logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
				logWarning("] failed (not found in properties, trying full scan from usergraph)");
		}

		// Bad, bad route, scans EVERY edge in the graph. Only when needed
		for (auto& u : adjList) {
			for (auto& v : u.second) {
				if (v.second.erase(edgeUuid)) {
					logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
						logVerbose("] successfully with warnings");

					if (!eraseProperties || (eraseProperties && props->eraseEdgeProp(edgeUuid)))
						return true;
					logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
						logWarning("] failed (edge prop deletion failed)");
					return false;
				}
			}
		}

		logInsert("UserGraph: Remove edge [") logInsert(edgeUuid)
			logWarning("] failed (edge not found, including in usergraph)");
	}
};
} // namespace gfn::core::usergraph
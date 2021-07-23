#pragma once

#include <unordered_map>
#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/VertexProp.hpp>
#include <Core/Properties/EdgeProp.hpp>
#include <System/Logging/Logging.hpp>

namespace gfn::core::properties {
/// @brief Stores all the properties such as positions, colors and names that will be accessed from outside for vertices
/// and edges
class Properties {
	// this class stays ar the bottom of anything, so that usergraph, core, the api and so on can access it
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::VertexProp> vertexPropList;
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::EdgeProp> edgePropList;

  public:
	/// @brief retrieve the properties object of the given vertex
	/// @returns nullptr if the prop does not exist, else the pointer to the prop object
	gfn::core::properties::VertexProp* getVertexProp(gfn::core::Uuid uuid, bool warnOnNotFound = true) {
		auto it = vertexPropList.find(uuid);
		if (it == vertexPropList.end()) {
			if (warnOnNotFound) {
				logInsert("Properties: Get vertex prop {") logInsert(uuid) logWarning("} failed (not found)");
			}
			return nullptr;
		}
		return &it->second;
	}

	/// @brief retrieve the properties object of the given edge
	/// @returns nullptr if the prop does not exist, else the pointer to the prop object
	gfn::core::properties::EdgeProp* getEdgeProp(gfn::core::Uuid uuid, bool warnOnNotFound = true) {
		auto it = edgePropList.find(uuid);
		if (it == edgePropList.end()) {
			if (warnOnNotFound) {
				logInsert("Properties: Get edge prop {") logInsert(uuid) logWarning("} failed (not found)");
			}
			return nullptr;
		}
		return &it->second;
	}

	/// @brief allocate a new vertex properties object
	/// @returns false if prop already exist but clearExisting is false
	bool newVertexProp(gfn::core::Uuid uuid, bool clearExisting = false) {
		if (clearExisting && vertexPropList.erase(uuid)) {
			logInsert("Properties: Cleared existing vertex prop {") logInsert(uuid) logVerbose("}");
		}

		if (vertexPropList.insert({uuid, gfn::core::properties::VertexProp()}).second) {
			logInsert("Properties: New vertex prop {") logInsert(uuid) logVerbose("}");
			getVertexProp(uuid)->uuid = uuid;
			return true;
		}
		logInsert("Properties: New vertex prop {") logInsert(uuid) logWarning("} failed (unexpectedly already exists)");
		return false;
	}

	/// @brief allocate a new vertex properties object
	bool newEdgeProp(gfn::core::Uuid uuid, bool clearExisting = false) {
		if (clearExisting && edgePropList.erase(uuid)) {
			logInsert("Properties: Cleared existing vertex prop {") logInsert(uuid) logVerbose("}");
		}

		if (edgePropList.insert({uuid, gfn::core::properties::EdgeProp()}).second) {
			logInsert("Properties: New edge prop {") logInsert(uuid) logVerbose("}");
			getEdgeProp(uuid)->edgeUuid = uuid;
			return true;
		}
		logInsert("Properties: New edge prop {") logInsert(uuid) logWarning("} failed (unexpectedly already exists)");
		return false;
	}

	/// @brief erase a vertex prop
	/// @returns if the erase was successful
	bool eraseVertexProp(gfn::core::Uuid uuid) {
		auto it = vertexPropList.find(uuid);
		if (it == vertexPropList.end()) {
			logInsert("Properties: Erase vertex prop {") logInsert(uuid) logWarning("} failed (not found)");
			return false;
		}
		vertexPropList.erase(it);
		logInsert("Properties: Erase vertex prop {") logInsert(uuid) logVerbose("}");
	}

	/// @brief erase an edge prop
	/// @returns if the erase was successful
	bool eraseEdgeProp(gfn::core::Uuid uuid) {
		auto it = edgePropList.find(uuid);
		if (it == edgePropList.end()) {
			logInsert("Properties: Erase edge prop {") logInsert(uuid) logWarning("} failed (not found)");
			return false;
		}
		edgePropList.erase(it);
		logInsert("Properties: Erase edge prop {") logInsert(uuid) logVerbose("}");
	}

	///@brief Returns the entire prop list, mainly for usergraph prop checkup
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::VertexProp>& getVertexPropList() {
		return vertexPropList;
	}

	///@brief Returns the entire prop list, mainly for usergraph prop checkup
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::EdgeProp>& getEdgePropList() { return edgePropList; }
};
} // namespace gfn::core::properties
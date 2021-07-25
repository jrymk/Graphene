#pragma once

#include <unordered_map>
#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/VertexProp.hpp>
#include <Core/Properties/EdgeProp.hpp>
#include <Core/Logging/Logging.hpp>

namespace gfn::core::properties {
/// @brief Stores all the properties such as positions, colors and names that will be accessed from outside for vertices
/// and edges
class Properties {
	gfn::core::logging::LogBuffer* logBuffer;
	// this class stays ar the bottom of anything, so that usergraph, core, the api and so on can access it
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::VertexProp> vertexPropList;
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::EdgeProp> edgePropList;

  public:
	void bindLogBuffer(gfn::core::logging::LogBuffer* logBuffer) { this->logBuffer = logBuffer; }

	/// @brief retrieve the properties object of the given vertex
	/// @returns nullptr if the prop does not exist, else the pointer to the prop object
	gfn::core::properties::VertexProp* getVertexProp(gfn::core::Uuid uuid, bool warnOnNotFound = true) {
		auto it = vertexPropList.find(uuid);
		if (it == vertexPropList.end()) {
			if (warnOnNotFound) {
				logMessage << "Properties: Get vertex prop {" << uuid << "} failed (not found)";
				logWarning;
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
				logMessage << "Properties: Get edge prop {" << uuid << "} failed (not found)";
				logWarning;
			}
			return nullptr;
		}
		return &it->second;
	}

	/// @brief allocate a new vertex properties object
	/// @returns false if prop already exist but clearExisting is false
	bool newVertexProp(gfn::core::Uuid uuid, bool clearExisting = false) {
		if (clearExisting && vertexPropList.erase(uuid)) {
			logMessage << "Properties: Cleared existing vertex prop {" << uuid << "}";
			logVerbose;
		}

		if (vertexPropList.insert({uuid, gfn::core::properties::VertexProp()}).second) {
			logMessage << "Properties: New vertex prop {" << uuid << "} at " << getVertexProp(uuid)->position;
			logInfo;
			getVertexProp(uuid)->uuid = uuid;
			return true;
		}
		logMessage << "Properties: New vertex prop {" << uuid << "} failed (unexpectedly already exists)";
		logWarning;
		return false;
	}

	/// @brief allocate a new vertex properties objectx
	bool newEdgeProp(gfn::core::Uuid uuid, bool clearExisting = false) {
		if (clearExisting && edgePropList.erase(uuid)) {
			logMessage << "Properties: Cleared existing vertex prop {" << uuid << "}";
			logVerbose;
		}

		if (edgePropList.insert({uuid, gfn::core::properties::EdgeProp()}).second) {
			logMessage << "Properties: New edge prop {" << uuid << "}";
			logVerbose;
			getEdgeProp(uuid)->edgeUuid = uuid;
			return true;
		}
		logMessage << "Properties: New edge prop {" << uuid << "} failed (unexpectedly already exists)";
		logWarning;
		return false;
	}

	/// @brief erase a vertex prop
	/// @returns if the erase was successful
	bool eraseVertexProp(gfn::core::Uuid uuid) {
		auto it = vertexPropList.find(uuid);
		if (it == vertexPropList.end()) {
			logMessage << "Properties: Erase vertex prop {" << uuid << "} failed (not found)";
			logWarning;
			return false;
		}
		vertexPropList.erase(it);
		logMessage << "Properties: Erase vertex prop {" << uuid << "}";
		logVerbose;
	}

	/// @brief erase an edge prop
	/// @returns if the erase was successful
	bool eraseEdgeProp(gfn::core::Uuid uuid) {
		auto it = edgePropList.find(uuid);
		if (it == edgePropList.end()) {
			logMessage << "Properties: Erase edge prop {" << uuid << "} failed (not found)";
			logWarning;
			return false;
		}
		edgePropList.erase(it);
		logMessage << "Properties: Erase edge prop {" << uuid << "}";
		logVerbose;
	}

	///@brief Returns the entire prop list, mainly for usergraph prop checkup
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::VertexProp>& getVertexPropList() {
		return vertexPropList;
	}

	///@brief Returns the entire prop list, mainly for usergraph prop checkup
	std::unordered_map<gfn::core::Uuid, gfn::core::properties::EdgeProp>& getEdgePropList() { return edgePropList; }
};
} // namespace gfn::core::properties
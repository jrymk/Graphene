#pragma once

#include <unordered_map>
#include <Objects/Uuid.hpp>
#include <Properties/UserProps.hpp>
#include <Properties/VertexProps.hpp>
#include <Properties/EdgeProps.hpp>
#include <System/Random/Random.hpp>
#include <Logging/Logging.hpp>
#include <Objects/Command.hpp>

namespace gfn::properties {
/// @brief Stores all the properties such as positions, colors and force values for internal use
    class Properties {
        // user friendly name mapping for uuids
        std::unordered_map<std::string, gfn::Uuid> accessNames;
        // this class stays ar the bottom of anything, so that usergraph, core, the api and so on can access it
        std::unordered_map<gfn::Uuid, std::pair<gfn::properties::VertexProps, gfn::properties::VertexPropsInternal>> vertexPropsList;
        std::unordered_map<gfn::Uuid, std::pair<gfn::properties::EdgeProps, gfn::properties::EdgePropsInternal>> edgePropsList;

    public:
        /// @brief uuids can be hard to type and read, user can assign custom names to every uuid for easy access
        bool assignAccessName(const std::string& accessName, const gfn::Uuid& uuid, bool overwrite = false) {
            if (!uuid::isUuid(uuid)) {
                // not a uuid
                return false;
            }
            auto f = accessNames.find(accessName);
            if (f != accessNames.end()) {
                if (overwrite) {
                    f->second = uuid;
                    return true;
                } else {
                    // access name already exist
                    return false;
                }
            }
            accessNames.insert({accessName, uuid});
            return true;
        }

        gfn::Uuid convertAccessName(const std::string& accessName) {
            auto f = accessNames.find(accessName);
            if (f != accessNames.end())
                return f->second;
            else {
                return "";
                // access name is not assigned
            }
        }

        /// @brief retrieve the properties object of the given vertex
        /// @returns first: vertex prop, second: vertex prop internal; nullptr if the prop does not exist, else the pointer to the prop object
        std::pair<gfn::properties::VertexProps*, gfn::properties::VertexPropsInternal*>
        getVertexProps(const gfn::Uuid& uuid, bool warnOnNotFound = true) {
            auto it = vertexPropsList.find(uuid);
            if (it == vertexPropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get core vertex prop {" << uuid << "} failed (not found)";
                    logWarning;
                }
                return {nullptr, nullptr};
            }
            return {&it->second.first, &it->second.second};
        }

        /// @brief retrieve the properties object of the given edge
        /// @first: edge prop, second: edge prop internal; returns nullptr if the prop does not exist, else the pointer to the prop object
        std::pair<gfn::properties::EdgeProps*, gfn::properties::EdgePropsInternal*>
        getEdgeProps(const gfn::Uuid& edgeUuid, bool warnOnNotFound = true) {
            auto it = edgePropsList.find(edgeUuid);
            if (it == edgePropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get core edge prop {" << edgeUuid << "} failed (not found)";
                    logWarning;
                }
                return {nullptr, nullptr};
            }
            return {&it->second.first, &it->second.second};
        }

        /// @brief allocate a new vertex properties object
        /// @returns false if prop already exist but clearExisting is false
        bool newVertexProps(const gfn::Uuid& uuid, bool clearExisting = false) {
            std::cerr << "new prop!\n";
            if (clearExisting && vertexPropsList.erase(uuid)) {
                logMessage << "Properties: Cleared existing core vertex prop {" << uuid << "}";
                logVerbose;
            }

            auto result = vertexPropsList.insert(
                    {uuid, {gfn::properties::VertexProps(), gfn::properties::VertexPropsInternal()}});
            if (result.second) {
                std::uniform_real_distribution dis(-20.0, 20.0);
                result.first->second.first.position = gfn::Vec2f(dis(gfn::system::random::getEngine()),
                                                                 dis(gfn::system::random::getEngine()));
                logMessage << "Properties: New core vertex prop {" << uuid << "} at "
                           << getVertexProps(uuid).first->position;
                logInfo;
                getVertexProps(uuid).first->uuid = uuid;
                return true;
            }
            logMessage << "Properties: New vertex prop {" << uuid << "} failed (unexpectedly already exists)";
            logWarning;
            return false;
        }

        /// @brief allocate a new vertex properties object
        bool newEdgeProps(const gfn::Uuid& edgeUuid, bool clearExisting = false) {
            if (clearExisting && edgePropsList.erase(edgeUuid)) {
                logMessage << "Properties: Cleared existing core edge prop {" << edgeUuid << "}";
                logVerbose;
            }

            auto result = edgePropsList.insert(
                    {edgeUuid, {gfn::properties::EdgeProps(), gfn::properties::EdgePropsInternal()}});
            if (result.second) {
                result.first->second.first.edgeUuid = edgeUuid;
                logMessage << "Properties: New core edge prop {" << edgeUuid << "}";
                logVerbose;
                return true;
            }
            logMessage << "Properties: New core edge prop {" << edgeUuid << "} failed (unexpectedly already exists)";
            logWarning;
            return false;
        }

        /// @brief erase a vertex prop
        /// @returns if the erase was successful
        bool eraseVertexProps(const gfn::Uuid& edgeUuid) {
            auto it = vertexPropsList.find(edgeUuid);
            if (it == vertexPropsList.end()) {
                logMessage << "Properties: Erase core vertex prop {" << edgeUuid << "} failed (not found)";
                logWarning;
                return false;
            }
            vertexPropsList.erase(it);
            logMessage << "Properties: Erase core vertex prop {" << edgeUuid << "}";
            logVerbose;
            return true;
        }

        /// @brief erase an edge prop
        /// @returns if the erase was successful
        bool eraseEdgeProps(const gfn::Uuid& edgeUuid) {
            auto it = edgePropsList.find(edgeUuid);
            if (it == edgePropsList.end()) {
                logMessage << "Properties: Erase core edge prop {" << edgeUuid << "} failed (not found)";
                logWarning;
                return false;
            }
            edgePropsList.erase(it);
            logMessage << "Properties: Erase core edge prop {" << edgeUuid << "}";
            logVerbose;
            return true;
        }

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, std::pair<gfn::properties::VertexProps, gfn::properties::VertexPropsInternal>>&
        getVertexPropList() {
            return vertexPropsList;
        }

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, std::pair<gfn::properties::EdgeProps, gfn::properties::EdgePropsInternal>>&
        getEdgePropList() { return edgePropsList; }

        void exportToUserProps(gfn::properties::UserProps* destination) {
            destination->getVertexPropList().clear();
            for (auto& v : vertexPropsList)
                destination->getVertexPropList().insert({v.first, v.second.first});

            destination->getEdgePropList().clear();
            for (auto& e : edgePropsList) {
                e.second.first.startVertexPosition = getVertexProps(e.second.first.startVertexUuid).first->position;
                e.second.first.endVertexPosition = getVertexProps(e.second.first.endVertexUuid).first->position;
                destination->getEdgePropList().insert({e.first, e.second.first});
            }
        }
    };
} // namespace gfn::properties
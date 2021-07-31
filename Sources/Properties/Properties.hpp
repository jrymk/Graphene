#pragma once

#include <unordered_map>
#include <Objects/Uuid.hpp>
#include <Properties/VertexProps.hpp>
#include <Properties/EdgeProps.hpp>
#include <System/Random/Random.hpp>
#include <Logging/Logging.hpp>
#include <Objects/Command.hpp>
#include <json.hpp>

namespace gfn::props {
/// @brief Stores all the properties such as positions, colors and force values for internal use
    class Properties {
        // user friendly name mapping for uuids
        std::unordered_map<std::string, gfn::Uuid> accessNames;
        // this class stays ar the bottom of anything, so that usergraph, core, the api and so on can access it
        std::unordered_map<gfn::Uuid, gfn::props::VertexProps> vertexPropsList;
        std::unordered_map<gfn::Uuid, gfn::props::EdgeProps> edgePropsList;

    public:
        void serialize(nlohmann::json& j) {
            j["vertices"] = nullptr;
            j["edges"] = nullptr;
            for (auto &v : vertexPropsList)
                v.second.serialize(j["vertices"][v.first]);
            for (auto &e : edgePropsList)
                e.second.serialize(j["edges"][e.first]);
        }


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
        /// @returns nullptr if the prop does not exist, else the pointer to the prop object
        gfn::props::VertexProps* getVertexProps(const gfn::Uuid& uuid, bool warnOnNotFound = true) {
            auto it = vertexPropsList.find(uuid);
            if (it == vertexPropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get core vertex prop {" << uuid << "} failed (not found)";
                    logWarning;
                }
                return nullptr;
            }
            return &it->second;
        }

        /// @brief retrieve the properties object of the given edge
        /// @returns: returns nullptr if the prop does not exist, else the pointer to the prop object
        gfn::props::EdgeProps* getEdgeProps(const gfn::Uuid& edgeUuid, bool warnOnNotFound = true) {
            auto it = edgePropsList.find(edgeUuid);
            if (it == edgePropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get core edge prop {" << edgeUuid << "} failed (not found)";
                    logWarning;
                }
                return nullptr;
            }
            return &it->second;
        }

        /// @brief allocate a new vertex properties object
        /// @returns false if prop already exist but clearExisting is false
        bool newVertexProps(const gfn::Uuid& uuid, bool clearExisting = false) {
            std::cerr << "new prop!\n";
            if (clearExisting && vertexPropsList.erase(uuid)) {
                logMessage << "Properties: Cleared existing core vertex prop {" << uuid << "}";
                logVerbose;
            }

            auto result = vertexPropsList.insert({uuid, gfn::props::VertexProps()});
            if (result.second) {
                std::uniform_real_distribution dis(-20.0, 20.0);
                result.first->second.position.value = gfn::Vec2f(dis(gfn::system::random::getEngine()),
                                                                 dis(gfn::system::random::getEngine()));
                logMessage << "Properties: New core vertex prop {" << uuid << "} at " <<
                           getVertexProps(uuid)->position.value;
                logInfo;
                getVertexProps(uuid)->uuid.value = uuid;
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

            auto result = edgePropsList.insert({edgeUuid, gfn::props::EdgeProps()});
            if (result.second) {
                result.first->second.edgeUuid.value = edgeUuid;
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
        std::unordered_map<gfn::Uuid, gfn::props::VertexProps>& getVertexPropList() {
            return vertexPropsList;
        }

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, gfn::props::EdgeProps>&
        getEdgePropList() { return edgePropsList; }
    };
} // namespace gfn::properties
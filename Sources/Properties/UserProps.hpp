#pragma once

#include <unordered_map>
#include <Objects/Uuid.hpp>
#include <Properties/VertexProps.hpp>
#include <Properties/EdgeProps.hpp>
#include <Logging/Logging.hpp>

namespace gfn::properties {
/// @brief Stores all the properties such as positions, colors and names that will be accessed from outside for vertices
/// and edges, and will be copied every interface buffer swap
    class UserProps {
        // data in: core buffer dump
        // data out: editor graph renderer and controls, api
        std::unordered_map<gfn::Uuid, VertexProps> vertexPropsList;
        std::unordered_map<gfn::Uuid, EdgeProps> edgePropsList;

    public:
        /// @brief retrieve the properties object of the given vertex
        /// @returns first: vertex prop, second: vertex prop internal; nullptr if the prop does not exist, else the pointer to the prop object
        VertexProps* getVertexProps(const gfn::Uuid& uuid, bool warnOnNotFound = true) {
            auto it = vertexPropsList.find(uuid);
            if (it == vertexPropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get vertex prop {" << uuid << "} failed (not found)";
                    logWarning;
                }
                return nullptr;
            }
            return &it->second;
        }

        /// @brief retrieve the properties object of the given edge
        /// @first: edge prop, second: edge prop internal; returns nullptr if the prop does not exist, else the pointer to the prop object
        EdgeProps* getEdgeProps(const gfn::Uuid& uuid, bool warnOnNotFound = true) {
            auto it = edgePropsList.find(uuid);
            if (it == edgePropsList.end()) {
                if (warnOnNotFound) {
                    logMessage << "Properties: Get edge prop {" << uuid << "} failed (not found)";
                    logWarning;
                }
                return nullptr;
            }
            return &it->second;
        }

        // The writing of this data structure will be handled by one single function in core.

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, VertexProps>& getVertexPropList() {
            return vertexPropsList;
        }

        ///@brief Returns the entire prop list, mainly for usergraph prop checkup
        std::unordered_map<gfn::Uuid, EdgeProps>& getEdgePropList() {
            return edgePropsList;
        }
    };
}
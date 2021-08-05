#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Properties/Properties.h>
#include <Objects/Uuid.h>
#include <Objects/Command.h>

namespace gfn {
    ///@brief usergraph is the place where the master graph structure is stored, stored in UUIDs, it is friendly to string
    /// command inputs, have more tolerance to misuse.
    class Graph {
    private:
        std::unordered_map<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>> adjList;

    public:
        gfn::Properties props;
        bool pendingUpdate = true;

        Graph();

        std::unordered_map<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>>& getAdjList();

        void serialize(binn* document);

        void deserialize(void* document);

        /// @brief Recommended to be called upon graph update, this function checks for missing props from Properties
        /// @param fix when encountered a prop mismatch, auto create the prop
        /// @returns true if everything is fine, will return false even if it is fixed
        bool validateProps(bool fix = true);

        /// @brief add a new vertex with auto-generated uuid represents the vertex created
        void addVertex(gfn::Command command, gfn::Command& output);

        /// @brief Deletes vertex with the given uuid. erase**Properties will also clear the properties tied to the uuid,
        /// without erase so you can create a new vertex/edge with the same uuid to retreive all the properties.
        /// removeAllAdjacentEdges remove edges that points to the vertex getting deleted
        /// @returns if the deleting was successful
        void removeVertex(gfn::Command command, gfn::Command& output);

        // add a new edge with auto-generated uuid between two given vertices
        /// @returns a pair with first indicating if it was successful and second the uuid auto-generated that
        /// represents the edge created
        void addEdge(gfn::Command command, gfn::Command& output);

        /// @brief remove edge with specified uuid (one edge)
        /// @returns if the edge deleting is successful or not (edge not found). If eraseProperties is on, false will
        /// also be
        /// returned if properties not found (properties delete unsuccessful)
        void removeEdge(gfn::Command command, gfn::Command& output);
    };
}
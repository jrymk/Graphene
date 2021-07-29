#pragma once

#include <unordered_map>
#include <unordered_set>
#include <atomic>
#include <Objects/Uuid.hpp>
#include <Logging/Logging.hpp>
#include <Properties/Properties.hpp>
#include <Objects/Command.hpp>

///@brief usergraph is the place where the master graph structure is stored, stored in UUIDs, it is friendly to string
/// command inputs, have more tolerance to misuse.
namespace gfn::usergraph {
    class UserGraph {
        std::unordered_map<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>> adjList;
        gfn::logging::LogBuffer* logBuffer;
        gfn::properties::Properties* properties;

    public:
        UserGraph() {}

        std::atomic_bool pendingUpdate = true;

        void bindLogBuffer(gfn::logging::LogBuffer* logBuffer) { this->logBuffer = logBuffer; }

        void bindProperties(gfn::properties::Properties* _properties) { properties = _properties; }

        std::unordered_map<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>>& getAdjList() {
            return adjList;
        }

        gfn::Uuid execute(const Command& cmd) {
            gfn::command::Parser parser(cmd);
            auto op = parser.readWord();
            if (op == "new") {
                auto it = parser.readWord();
                if (it == "vertex")
                    addVertexCmd(parser.getRemaining());
                if (it == "edge")
                    addEdgeCmd(parser.getRemaining());
            }
            if (op == "delete") {

            }
        }

        ///@brief Recommended to be called upon graph update, this function checks for missing props from Properties
        /// @param fix when encountered a prop mismatch, auto create the prop
        /// @returns true if everything is fine, will return false even if it is fixed
        bool validateProps(bool fix = true) {
            int problems = 0;

            auto vertexPropList = properties->getVertexPropList();
            auto edgePropList = properties->getEdgePropList();

            for (auto& v : vertexPropList) {
                if (v.second.first.uuid != v.first) {
                    problems++;
                    /*logMessage << "UserGraph: Props checkup error found: Vertex {" << v.second.first.uuid
                               << "} prop uuid does not match up with the key in props list " << v.first;
                    logWarning*/;
                    if (fix)
                        v.second.first.uuid = v.first;
                }
            }
            for (auto& e : edgePropList) {
                if (e.second.first.edgeUuid != e.first) {
                    problems++;
                    /*logMessage << "UserGraph: Props checkup error found: Edge {" << e.second.first.edgeUuid
                               << "} prop uuid does not match up with the key in props list " << e.first;
                    logWarning*/;
                    if (fix)
                        e.second.first.edgeUuid = e.first;
                }
            }

            // checking for missing props
            std::unordered_set<gfn::Uuid> invalidVertexList;
            std::unordered_set<gfn::Uuid> invalidEdgeList;

            for (auto& u : adjList) {
                invalidVertexList.insert(u.first);
                for (auto& v : u.second) {
                    for (auto& e : v.second) {
                        invalidEdgeList.insert(e);
                    }
                }
            }

            // at this point invalid lists is a whole list of vertex/edges in the usergraph. we use it to mark the props if
            // they are valid
            for (auto& p : vertexPropList) {
                if (invalidVertexList.find(p.first) == invalidVertexList.end())
                    p.second.first.enabled = false;
                else
                    p.second.first.enabled = true;
            }

            for (auto it = invalidVertexList.begin(); it != invalidVertexList.end();) {
                if (vertexPropList.find(*it) != vertexPropList.end())
                    it = invalidVertexList.erase(it);
                else
                    it++;
            }
            for (auto it = invalidEdgeList.begin(); it != invalidEdgeList.end();) {
                if (edgePropList.find(*it) != edgePropList.end())
                    it = invalidEdgeList.erase(it);
                else
                    it++;
            }

            if (!invalidVertexList.empty()) {
                for (auto& v : invalidVertexList) {
                    problems++;
                    /*logMessage << "UserGraph: Props checkup error found: Vertex {" << v << "} prop does not exist";
                    logWarning*/;
                    if (fix)
                        properties->newVertexProps(v);
                }
            }
            if (!invalidEdgeList.empty()) {
                for (auto& e : invalidEdgeList) {
                    problems++;
                    /*logMessage << "UserGraph: Props checkup error found: Edge {" << e << "} prop does not exist";
                    logWarning*/;
                    if (fix)
                        properties->newEdgeProps(e);
                }
            }

            if (problems) {
                /*logMessage << "UserGraph: Props checkup completed: Found " << problems << " errors in total";
                logWarning*/;
                return false;
            }
            /*logMessage << "UserGraph: Props checkup completed: Found " << problems << "errors in total";
            logVerbose*/;
            return true;
        }

        // add a new vertex with auto-generated uuid
        /// @returns  a pair with first indicating if it was successful and second the uuid auto-generated that
        /// represents the vertex created
        std::pair<bool, gfn::Uuid> addVertex() {
            pendingUpdate = true;
            for (int retries = 1; retries <= 5; retries++) {
                auto uuid = gfn::uuid::createUuid();
                if (adjList.insert({uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>()}).second) {
                    // add entry to adjacency list
                    properties->newVertexProps(uuid);
                    /*logMessage << "UserGraph: Added new vertex {" << uuid << "} (auto-generated)";
                    logInfo*/;
                    return {true, uuid};
                }
                // props already existed somehow
                /*logMessage << "UserGraph: Add new vertex {" << uuid
                           << "} (auto-generated) failed (already exists, retrying: " << std::to_string(retries)
                           << "/5)";
                logWarning*/;
            }
            /*logMessage << "UserGraph: (unexpected uuid creation failure)";
            logError*/;
            return {false, gfn::uuid::createNil()};
        }

        // add a new vertex with the given uuid
        bool addVertex(const gfn::Uuid& uuid) {
            pendingUpdate = true;
            if (adjList.insert({uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>()}).second) {
                // add entry to adjacency list
                properties->newVertexProps(uuid);
                /*logMessage << "UserGraph: Added new vertex {" << uuid << "}";
                logInfo*/;
                return true;
            }
            /*logMessage << "UserGraph: Add new vertex {" << uuid << "} failed (uuid given already exists)";
            logError*/;
            return false;
        }

        void addVertexCmd(const gfn::Command& cmd) {
            gfn::command::Parser parser(cmd);
            auto id = parser.readWord();
            if (id.empty())
                addVertex();
            else
                addVertex(id);
        }

        ///@brief Deletes vertex with the given uuid. erase**Properties will also clear the properties tied to the uuid,
        /// without erase so you can create a new vertex/edge with the same uuid to retreive all the properties.
        /// removeAllAdjacentEdges remove edges that points to the vertex getting deleted
        /// @returns if the deleting was successful
        bool removeVertex(const gfn::Uuid& uuid, bool eraseVertexProperties = true, bool eraseEdgeProperties = true,
                          bool removeAllAdjacentEdges = true) {
            pendingUpdate = true;
            auto uIt = adjList.find(uuid);
            if (uIt == adjList.end()) {
                /*logMessage << "UserGraph: Remove vertex {" << uuid << "} failed (not found)";
                logWarning*/;
                return false;
            }

            /*logMessage << "UserGraph: Removing edges adjacent to vertex {" << uuid << "} with eraseEdgeProperties "
                       << (eraseEdgeProperties ? "enabled" : "disabled");
            logVerbose*/;
            if (eraseEdgeProperties) {
                for (auto& v : uIt->second) {
                    /*logMessage << "    vertex {" << v.first << "}";
                    logVerbose*/;
                    for (auto& e : v.second) {
                        /*logMessage << "        edge {" << e << "}";
                        logVerbose*/;
                        /// TODO: erase edge property
                    }
                }
            }

            if (removeAllAdjacentEdges) {
                /*logMessage << "  <edges TO the vertex>";
                logVerbose*/;
                for (auto& u : adjList) {
                    if (u.first != uuid) {
                        for (auto& v : u.second) {
                            if (v.first == uuid) {
                                /*logMessage << "    from vertex {" << u.first << "}";
                                logVerbose*/;
                                if (eraseEdgeProperties) {
                                    for (auto& e : v.second) {
                                        /*logMessage << "        edge {" << e << "}";
                                        logVerbose*/;
                                        /// TODO: erase edge property
                                    }
                                }
                                u.second.erase(uuid); // erase vertex-edge entries from other vertices
                            }
                        }
                    }
                }
            }

            /*logMessage << "UserGraph: Removing vertex {" << uuid << "} with eraseVertexProperties "
                       << (eraseVertexProperties ? "enabled" : "disabled");
            logInfo*/;
            if (eraseVertexProperties)
                properties->eraseVertexProps(uuid);

            adjList.erase(uuid);
        }

        // add a new edge with auto-generated uuid between two given vertices
        /// @returns a pair with first indicating if it was successful and second the uuid auto-generated that
        /// represents the edge created
        std::pair<bool, gfn::Uuid> addEdge(const gfn::Uuid& startVertex, const gfn::Uuid& endVertex) {
            pendingUpdate = true;
            auto startIt = adjList.find(startVertex);
            if (startIt == adjList.end()) {
                /*logMessage << "UserGraph: Add edge failed (startVertex {" << startVertex << "} does not exist";
                logWarning*/;
                return {false, gfn::uuid::createNil()};
            }

            auto endIt = adjList.find(endVertex);
            if (endIt == adjList.end()) {
                /*logMessage << "UserGraph: Add edge failed (endVertex {" << endVertex << "} does not exist";
                logWarning*/;
                return {false, gfn::uuid::createNil()};
            }

            for (int retries = 1; retries <= 5; retries++) {
                auto edgeId = gfn::uuid::createUuid();
                if (!properties->getEdgeProps(edgeId, false).first) { // props does not exist
                    // add entry to adjacency list
                    if (startIt->second.insert({endVertex, std::unordered_set<gfn::Uuid>()}).second) {
                        // created new endVertex entry
                    }
                    if (endIt->second.insert({startVertex, std::unordered_set<gfn::Uuid>()}).second) {
                        // created new endVertex entry
                    }

                    if (startIt->second.find(endVertex)->second.insert(edgeId).second) {
                        /*logMessage << "UserGraph: Added new edge with startVertex {" << startVertex << "} endVertex {"
                                   << endVertex << "} edge {" << edgeId << "} successfully";
                        logInfo*/;

                        properties->newEdgeProps(edgeId);
                        properties->getEdgeProps(edgeId).first->startVertexUuid = startVertex;
                        properties->getEdgeProps(edgeId).first->endVertexUuid = endVertex;
                        properties->getEdgeProps(edgeId).first->edgeUuid = edgeId;
                        return {true, edgeId};
                    }
                    /*logMessage << "UserGraph: Add new edge with startVertex {" << startVertex << "} endVertex {"
                               << endVertex << "} edge {" << edgeId << "} failed (unexpected insertion failure)";
                    logError*/;
                    return {false, gfn::uuid::createNil()};
                }
                // props already existed somehow
                /*logMessage << "UserGraph: Add new edge with startVertex {" << startVertex << "} endVertex {"
                           << endVertex
                           << "} failed (edge {" << edgeId
                           << "} (auto-generated) already exists, retrying: " << std::to_string(retries) << "/5)";
                logWarning*/;
            }
            /*logMessage << "UserGraph: (unexpected uuid creation failure)";
            logError*/;
            return {false, gfn::uuid::createNil()};
        }

        // add a new edge with the given uuid between two given vertices
        bool addEdge(const gfn::Uuid& startVertex, const gfn::Uuid& endVertex, const gfn::Uuid& edgeUuid) {
            pendingUpdate = true;
            auto startIt = adjList.find(startVertex);
            if (startIt == adjList.end()) {
                /*logMessage << "UserGraph: Add edge failed (startVertex {" << startVertex << "} does not exist";
                logWarning*/;
                return false;
            }

            auto endIt = adjList.find(endVertex);
            if (endIt == adjList.end()) {
                /*logMessage << "UserGraph: Add edge failed (endVertex {" << endVertex << "} does not exist";
                logWarning*/;
                return false;
            }

            if (!properties->getEdgeProps(edgeUuid, false).first) { // props does not exist
                // add entry to adjacency list
                if (startIt->second.insert({endVertex, std::unordered_set<gfn::Uuid>()}).second) {
                    // created new endVertex entry
                }
                if (endIt->second.insert({startVertex, std::unordered_set<gfn::Uuid>()}).second) {
                    // created new endVertex entry
                }

                if (startIt->second.find(endVertex)->second.insert(edgeUuid).second) {
                    /*logMessage << "UserGraph: Added new edge with startVertex {" << startVertex << "} endVertex {"
                               << endVertex << "} edge {" << edgeUuid << "} successfully";
                    logInfo*/;

                    properties->newEdgeProps(edgeUuid);
                    properties->getEdgeProps(edgeUuid).first->startVertexUuid = startVertex;
                    properties->getEdgeProps(edgeUuid).first->endVertexUuid = endVertex;
                    properties->getEdgeProps(edgeUuid).first->edgeUuid = edgeUuid;
                    return true;
                }

                /*logMessage << "UserGraph: Add new edge with startVertex {" << startVertex << "} endVertex {"
                           << endVertex
                           << "} edge {" << edgeUuid << "} failed (unexpected insertion failure)";
                logError*/;
                return false;
            }
            /*logMessage << "UserGraph: Add new edge with startVertex {" << startVertex << "} endVertex {" << endVertex
                       << "} edge {" << edgeUuid << "} failed (edge uuid given already exists)";
            logWarning*/;
            return false;
        }

        void addEdgeCmd(const gfn::Command& cmd) {
            gfn::command::Parser parser(cmd);
            auto sv = parser.readWord();
            auto ev = parser.readWord();
            auto ei = parser.readWord();
            if (!sv.empty() && !ev.empty()) {
                if (ei.empty())
                    addEdge(sv, ev);
                else
                    addEdge(sv, ev, ei);
            }
        }

        ///@brief remove edge with specified uuid (one edge)
        ///@returns if the edge deleting is successful or not (edge not found). If eraseProperties is on, false will
        /// also be
        /// returned if properties not found (properties delete unsuccessful)
        bool removeEdge(const gfn::Uuid& edgeUuid, bool eraseProperties) {
            pendingUpdate = true;
            auto prop = properties->getEdgeProps(edgeUuid);
            if (prop.first) { // props exists
                // the "use start and end vertices from props" route
                auto startIt = adjList.find(prop.first->startVertexUuid);
                if (startIt != adjList.end()) {
                    auto endIt = startIt->second.find(prop.first->endVertexUuid);
                    if (endIt != startIt->second.end()) {
                        if (endIt->second.erase(edgeUuid)) {
                            // erased the edge successfully
                            if (endIt->second.empty()) {
                                // no more edges between the given start-end vertex pair
                                startIt->second.erase(endIt);
                                if (!eraseProperties || (eraseProperties && properties->eraseEdgeProps(edgeUuid))) {
                                    /*logMessage << "UserGraph: Successfully removed edge {" << edgeUuid << "}";
                                    logInfo*/;
                                    return true;
                                }
                                /*logMessage << "UserGraph: Remove edge {" << edgeUuid
                                           << "} failed (edge prop deletion failed)";
                                logWarning*/;
                                return false;
                            }
                            /*logMessage << "UserGraph: Successfully removed edge {" << edgeUuid << "}";
                            logInfo*/;
                            // still have edges between the given start-end vertex pair
                            return true;
                        }
                        // can find start and end vertex entries but edge deletion failed, entering full scan route
                        /*logMessage
                                << "UserGraph: Remove edge {" << edgeUuid
                                << "} failed (vertices from prop found but not the edge, trying full scan from usergraph)";
                        logWarning*/;
                    }
                    /*logMessage << "UserGraph: Remove edge {" << edgeUuid
                               << "} failed (endVertex from prop entry not found from startVertex, trying full scan "
                                  "from usergraph)";
                    logWarning*/;
                }
                /*logMessage << "UserGraph: Remove edge {" << edgeUuid
                           << "} failed (startVertex from prop not found, trying full scan from usergraph)";
                logWarning*/;
            } else {
                /*logMessage << "UserGraph: Remove edge {" << edgeUuid
                           << "} failed (not found in properties, trying full scan from usergraph)";
                logWarning*/;
            }

            // Bad, bad route, scans EVERY edge in the graph. Only when needed
            for (auto& u : adjList) {
                for (auto& v : u.second) {
                    if (v.second.erase(edgeUuid)) {
                        if (!eraseProperties || (eraseProperties && properties->eraseEdgeProps(edgeUuid))) {
                            /*logMessage << "UserGraph: Remove edge {" << edgeUuid
                                       << "} successfully with warnings (completed with full scan)";
                            logInfo*/;
                            return true;
                        }
                        /*logMessage << "UserGraph: Remove edge {" << edgeUuid << "} failed (edge prop deletion
                        failed)";
                        logWarning*/;
                        return false;
                    }
                }
            }

            /*logMessage << "UserGraph: Remove edge {" << edgeUuid << "} failed (edge not found, including in "
                                                                     "usergraph)";
            logWarning*/;
        }
    };
} // namespace gfn::core::usergraph
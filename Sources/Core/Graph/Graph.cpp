#include "Graph.h"

namespace gfn {

    Graph::Graph() :
            props(),
            cfg() {
    }

    std::unordered_map<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>>& Graph::getAdjList() {
        return adjList;
    }

    void Graph::serialize(binn* document) {
        binn* uList = binn_list();
        for (const auto& u : adjList) {
            binn* uObj = binn_object();
            binn_object_set_str(uObj, "uUuid", std::string(u.first).data());
            binn* vList = binn_list();
            for (const auto& v : u.second) {
                binn* vObj = binn_object();
                binn_object_set_str(vObj, "vUuid", std::string(v.first).data());
                binn* eList = binn_list();
                for (auto e : v.second)
                    binn_list_add_str(eList, e.data());
                binn_object_set_list(vObj, "edges", eList);
                binn_free(eList);
                binn_list_add_object(vList, vObj);
                binn_free(vObj);
            }
            binn_object_set_list(uObj, "adjV", vList);
            binn_free(vList);
            binn_list_add_object(uList, uObj);
            binn_free(uObj);
        }
        binn_object_set_list(document, "adjList", uList);
        binn_free(uList);
    }

    void Graph::deserialize(void* document) {
        adjList.clear();
        void* uList = binn_object_list(document, "adjList");
        auto uCount = binn_count(uList);
        for (int u = 1; u <= uCount; u++) {
            void* uObj = binn_list_object(uList, u);
            std::pair<gfn::Uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>> uStruct;
            uStruct.first = binn_object_str(uObj, "uUuid");
            void* vList = binn_object_list(uObj, "adjV");
            auto vCount = binn_count(vList);
            for (int v = 1; v <= vCount; v++) {
                void* vObj = binn_list_object(vList, v);
                std::pair<gfn::Uuid, std::unordered_set<gfn::Uuid>> vStruct;
                vStruct.first = binn_object_str(vObj, "vUuid");
                void* eList = binn_object_list(vObj, "edges");
                auto eCount = binn_count(eList);
                for (int e = 1; e <= eCount; e++)
                    vStruct.second.insert(binn_list_str(eList, e));
                uStruct.second.insert(vStruct);
            }
            adjList.insert(uStruct);
        }
        validateProps(true);
        pendingUpdate = true;
    }

    bool Graph::validateProps(bool fix) {
        int problems = 0;

        auto vertexPropList = props.getVertexPropsList();
        auto edgePropList = props.getEdgePropsList();

        for (auto& v : vertexPropList) {
            if (v.second.uuid.value != v.first) {
                problems++;
                if (fix)
                    v.second.uuid.value = v.first;
            }
        }
        for (auto& e : edgePropList) {
            if (e.second.edgeUuid.value != e.first) {
                problems++;
                if (fix)
                    e.second.edgeUuid.value = e.first;
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
                p.second.enabled.value = false;
            else
                p.second.enabled.value = true;
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
                if (fix)
                    props.newVertexProps(v);
            }
        }
        if (!invalidEdgeList.empty()) {
            for (auto& e : invalidEdgeList) {
                problems++;
                if (fix)
                    props.newEdgeProps(e);
            }
        }

        if (problems)
            return false;
        return true;
    }

    void Graph::addVertex(gfn::Args command, gfn::Args& output) {
        pendingUpdate = true;

        gfn::Uuid uuid;
        if (isUuid(command.getParamValue("-uuid"))) {
            uuid = command.getParamValue("-uuid");
            output.newParam("-uuidsource", "user");
        } else {
            uuid = gfn::createUuid();
            output.newParam("-uuidsource", "gen");
        }
        if (!command.getParamValue("-name").empty()) {
            if (command.getFlag("-force"))
                props.assignAccessName(command.getParamValue("-name"), uuid, true);
            else {
                if (props.convertAccessName(command.getParamValue("-name")).empty())
                    props.assignAccessName(command.getParamValue("-name"), uuid);
                else {
                    output.newParam("-error", "ACCESS_NAME_ALREADY_DEFINED");
                    output.newParam("-fix", "Pick another access name");
                    output.newParam("-successful", "false");
                    return;
                }
            }
        }
        if (adjList.insert({uuid, std::unordered_map<gfn::Uuid, std::unordered_set<gfn::Uuid>>()}).second) {
            props.newVertexProps(uuid);
            props.getVertexProps(uuid)->enabled.value = true;
            output.newParam("-successful", "true");
            output.newParam("-uuid", uuid);
            return;
        }
        output.newParam("-successful", "false");
        output.newParam("-error", "VERTEX_ALREADY_EXIST");
    }

///@brief Deletes vertex with the given uuid. erase**Properties will also clear the properties tied to the uuid,
/// without erase so you can create a new vertex/edge with the same uuid to retreive all the properties.
/// removeAllAdjacentEdges remove edges that points to the vertex getting deleted
/// @returns if the deleting was successful
    void Graph::removeVertex(gfn::Args command, gfn::Args& output) {
        pendingUpdate = true;
        gfn::Uuid uuid;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            if (adjList.find(uuid) == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-uuid", uuid);
                output.newParam("-error", "VERTEX_UUID_NOT_FOUND");
                return;
            }
        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = props.convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
            if (adjList.find(uuid) == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-uuid", uuid);
                output.newParam("-error", "VERTEX_UUID_NOT_FOUND");
                return;
            }
        }
        auto uIt = adjList.find(uuid);
        bool erase = command.getFlag("-erase-edge-props");
        for (auto& v : uIt->second) {
            for (auto& e : v.second) {
                if (erase)
                    props.eraseEdgeProps(e);
                else
                    props.getEdgeProps(e)->enabled.value = false;
            }
        }

        if (!command.getFlag("-do-not-remove-edges")) {
            for (auto& u : adjList) {
                if (u.first != uuid) {
                    for (auto v = u.second.begin(); v != u.second.end();) {
                        if (v->first == uuid) {
                            for (auto& e : v->second) {
                                if (erase)
                                    props.eraseEdgeProps(e);
                                else
                                    props.getEdgeProps(e)->enabled.value = false;
                            }
                            v = u.second.erase(
                                    u.second.find(uuid)); // erase vertex-edge entries from other vertices
                        } else
                            v++;
                    }
                }
            }
        }
        if (command.getFlag("-erase-vertex-props"))
            props.eraseVertexProps(uuid);
        else
            props.getVertexProps(uuid)->enabled.value = false;
        adjList.erase(uuid);
        output.newParam("-successful", "true");
        output.newParam("-uuid", uuid);
    }

    void Graph::addEdge(gfn::Args command, gfn::Args& output) {
        pendingUpdate = true;

        gfn::Uuid startUuid;
        auto startParam = command.getParamValue("-u");
        if (!startParam.empty())
            startUuid = startParam;
        else if (!command.getParamValue("-uname").empty()) {
            startUuid = props.convertAccessName(command.getParamValue("-uname"));
            if (startUuid.empty()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "START_VERTEX_ACCESS_NAME_UNDEFINED");
                return;
            }
        } else {
            // no start vertex specified
            output.newParam("-successful", "false");
            output.newParam("-error", "START_VERTEX_UNSPECIFIED");
            output.newParam("-fix", "Specify start vertex with [-u] or [-uname]");
            return;
        }
        auto uIt = adjList.find(startUuid);
        if (uIt == adjList.end()) {
            output.newParam("-successful", "false");
            output.newParam("-error", "START_VERTEX_NOT_FOUND");
            return;
        }

        gfn::Uuid endUuid;
        auto endParam = command.getParamValue("-v");
        if (!endParam.empty())
            endUuid = endParam;
        else if (!command.getParamValue("-vname").empty()) {
            endUuid = props.convertAccessName(command.getParamValue("-vname"));
            if (endUuid.empty()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "END_VERTEX_ACCESS_NAME_UNDEFINED");
                return;
            }
        } else {
            // no start vertex specified
            output.newParam("-successful", "false");
            output.newParam("-error", "END_VERTEX_UNSPECIFIED");
            output.newParam("-fix", "Specify end vertex with [-v] or [-vname]");
            return;
        }
        auto vIt = adjList.find(endUuid);
        if (vIt == adjList.end()) {
            output.newParam("-successful", "false");
            output.newParam("-error", "END_VERTEX_NOT_FOUND");
            return;
        }

        gfn::Uuid edgeUuid;
        if (isUuid(command.getParamValue("-uuid"))) {
            edgeUuid = command.getParamValue("-uuid");
            output.newParam("-uuidsource", "user");
        } else {
            edgeUuid = gfn::createUuid();
            output.newParam("-uuidsource", "gen");
        }
        if (!command.getParamValue("-name").empty()) {
            if (command.getFlag("-force"))
                props.assignAccessName(command.getParamValue("-name"), edgeUuid, true);
            else {
                if (props.convertAccessName(command.getParamValue("-name")).empty())
                    props.assignAccessName(command.getParamValue("-name"), edgeUuid);
                else {
                    output.newParam("-error", "ACCESS_NAME_ALREADY_DEFINED");
                    output.newParam("-fix", "Pick another access name");
                    output.newParam("-successful", "false");
                    return;
                }
            }
        }
        if (!props.getEdgeProps(edgeUuid, false)) { // props does not exist
            // add entry to adjacency list
            if (uIt->second.insert({endUuid, std::unordered_set<gfn::Uuid>()}).second) {
                // created new endVertex entry
            }
            if (vIt->second.insert({startUuid, std::unordered_set<gfn::Uuid>()}).second) {
                // created new endVertex entry
            }
            if (uIt->second.find(endUuid)->second.insert(edgeUuid).second) {
                output.newParam("-uuid", edgeUuid);
                if (!props.newEdgeProps(edgeUuid)) {
                    output.newParam("-successful", "false");
                    output.newParam("-error", "EDGE_UUID_ALREADY_EXIST");
                    return;
                }
                auto p = props.getEdgeProps(edgeUuid);
                p->startVertexUuid.value = startUuid;
                p->endVertexUuid.value = endUuid;
                p->edgeUuid.value = edgeUuid;
                p->enabled.value = true;
                output.newParam("-u", startUuid);
                output.newParam("-v", endUuid);
                output.newParam("-successful", "true");
            }
        }
    }

    void Graph::removeEdge(gfn::Args command, gfn::Args& output) {
        pendingUpdate = true;

        gfn::Uuid edgeUuid;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            edgeUuid = command.getParamValue("-uuid");
        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            edgeUuid = props.convertAccessName(command.getParamValue("-name"));
            if (edgeUuid.empty()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        } else {
            // v to v mode
            gfn::Uuid startUuid;
            auto startParam = command.getParamValue("-u");
            if (!startParam.empty())
                startUuid = startParam;
            else if (!command.getParamValue("-uname").empty()) {
                startUuid = props.convertAccessName(command.getParamValue("-uname"));
                if (startUuid.empty()) {
                    output.newParam("-successful", "false");
                    output.newParam("-error", "START_VERTEX_ACCESS_NAME_UNDEFINED");
                    return;
                }
            } else {
                // no start vertex specified
                output.newParam("-successful", "false");
                output.newParam("-error", "START_VERTEX_UNSPECIFIED");
                output.newParam("-fix", "Specify start vertex with [-u] or [-uname]");
                return;
            }

            auto uIt = adjList.find(startUuid);
            if (uIt == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "START_VERTEX_NOT_FOUND");
                return;
            }

            gfn::Uuid endUuid;
            auto endParam = command.getParamValue("-v");
            if (!endParam.empty())
                endUuid = endParam;
            else if (!command.getParamValue("-vname").empty()) {
                endUuid = props.convertAccessName(command.getParamValue("-vname"));
                if (endUuid.empty()) {
                    output.newParam("-successful", "false");
                    output.newParam("-error", "END_VERTEX_ACCESS_NAME_UNDEFINED");
                    return;
                }
            } else {
                // no start vertex specified
                output.newParam("-successful", "false");
                output.newParam("-error", "END_VERTEX_UNSPECIFIED");
                output.newParam("-fix", "Specify end vertex with [-v] or [-vname]");
                return;
            }
            auto vIt = adjList.find(endUuid);
            if (vIt == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "END_VERTEX_NOT_FOUND");
                return;
            }

            // v to v mode
            auto sIt = adjList.find(startUuid);
            auto eIt = adjList.find(endUuid);
            if (sIt == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "START_VERTEX_NOT_FOUND");
                return;
            }
            if (eIt == adjList.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "END_VERTEX_NOT_FOUND");
                return;
            }
            auto sEIt = eIt->second.find(startUuid);
            auto eEIt = sIt->second.find(endUuid);
            if (sEIt == eIt->second.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "START_VERTEX_ENTRY_NOT_FOUND");
                return;
            }
            if (eEIt == sIt->second.end()) {
                output.newParam("-successful", "false");
                output.newParam("-error", "END_VERTEX_ENTRY_NOT_FOUND");
                return;
            }

            bool erase = command.getFlag("-erase-edge-props");
            for (auto& e : sEIt->second) {
                output.newParam("-edge-uuid", e);
                if (!erase) {
                    props.getEdgeProps(e)->enabled.value = false;
                    continue;
                }
                if (props.eraseEdgeProps(e)) {
                    output.newParam("-uuid", e);
                    continue;
                }
                output.newParam("-successful", "false");
                output.newParam("-error", "EDGE_PROPS_ERASE_FAILED");
                return;
            }
            for (auto& e : eEIt->second) {
                output.newParam("-edge-uuid", e);
                if (!erase) {
                    props.getEdgeProps(e)->enabled.value = false;
                    continue;
                }
                if (props.eraseEdgeProps(e)) {
                    output.newParam("-uuid", e);
                    continue;
                }
                output.newParam("-successful", "false");
                output.newParam("-error", "EDGE_PROPS_ERASE_FAILED");
                return;
            }
            sIt->second.erase(eEIt);
            eIt->second.erase(sEIt);

            output.newParam("-successful", "true");
            return;
            // v to v mode
        }

        // edge mode
        auto prop = props.getEdgeProps(edgeUuid);
        if (prop) { // props exists
            output.newParam("-uuid", edgeUuid);
            // the "use start and end vertices from props" route
            auto startIt = adjList.find(prop->startVertexUuid.value);
            if (startIt != adjList.end()) {
                gfn::Uuid startVertex = startIt->first;
                output.newParam("-u", startVertex);
                auto endIt = startIt->second.find(prop->endVertexUuid.value);
                if (endIt != startIt->second.end()) {
                    gfn::Uuid endVertex = endIt->first;
                    output.newParam("-v", endVertex);
                    if (endIt->second.erase(edgeUuid)) {
                        // erased the edge successfully

                        if (endIt->second.empty()) {
                            // no more edges between the given start-end vertex pair, perform bi-directional edge
                            // entry deletion
                            startIt->second.erase(endIt); // start -> end entry

                            auto rStartIt = adjList.find(endVertex);
                            if (rStartIt == adjList.end()) {
                                output.newParam("-error", "END_VERTEX_ENTRY_NOT_FOUND");
                                output.newParam("-successful", "false");
                                return;
                            }
                            auto rEndIt = rStartIt->second.find(startVertex);
                            if (rEndIt == rStartIt->second.end()) {
                                output.newParam("-error", "START_VERTEX_EDGE_ENTRY_NOT_FOUND");
                                output.newParam("-successful", "false");
                                return;
                            }
                            if (!rEndIt->second.empty()) { // check if edge set for end -> start is empty
                                output.newParam("-error", "EDGE_SET_REVERSE_DISAGREE");
                                output.newParam("-successful", "false");
                                return;
                            }
                            rStartIt->second.erase(rEndIt); // end -> start entry
                        }
                        // still got edges between start vertex and end vertex, do nothing

                        // prop deletion
                        bool erase = command.getFlag("-erase-edge-props");
                        if (!erase || props.eraseEdgeProps(edgeUuid)) {
                            if (!erase)
                                props.getEdgeProps(edgeUuid)->enabled.value = false;
                            output.newParam("-successful", "true");
                            return;
                        }
                        output.newParam("-successful", "false");
                        output.newParam("-error", "EDGE_PROPS_ERASE_FAILED");
                        return;
                        // prop deletion
                    }
                    output.newParam("-warning", "EDGE_DELETION_FAILED");
                }
                output.newParam("-warning", "END_VERTEX_NOT_CONNECTED");
            }
            output.newParam("-warning", "START_VERTEX_NOT_CONNECTED");
        } else
            output.newParam("-warning", "EDGE_PROP_NOT_FOUND");

        // Bad, bad route, scans EVERY edge in the graph. Only when needed
        output.newParam("-warning", "FULL_SCAN_MODE");
        for (auto& u : adjList) {
            for (auto& v : u.second) {
                if (v.second.erase(edgeUuid)) {
                    bool erase = command.getFlag("-erase-edge-props");

                    if (!erase || props.eraseEdgeProps(edgeUuid)) {
                        if (!erase)
                            props.getEdgeProps(edgeUuid)->enabled.value = false;
                        auto rStartIt = adjList.find(v.first);
                        if (rStartIt == adjList.end()) {
                            output.newParam("-error", "END_VERTEX_ENTRY_NOT_FOUND");
                            output.newParam("-successful", "false");
                            return;
                        }
                        auto rEndIt = rStartIt->second.find(u.first);
                        if (rEndIt == rStartIt->second.end()) {
                            output.newParam("-error", "START_VERTEX_EDGE_ENTRY_NOT_FOUND");
                            output.newParam("-successful", "false");
                            return;
                        }
                        if (adjList.find(v.first)->second.find(u.first)->second.empty())
                            rStartIt->second.erase(rEndIt);

                        output.newParam("-successful", "true");
                        return;
                    }
                    output.newParam("-successful", "false");
                    output.newParam("-error", "EDGE_PROPS_ERASE_FAILED");
                    return;
                }
            }
        }
        output.newParam("-successful", "false");
        output.newParam("-error", "EDGE_NOT_FOUND");
    }

}
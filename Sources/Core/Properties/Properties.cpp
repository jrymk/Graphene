#include "Properties.h"
#include <Core/Objects/Random.h>

using namespace gfn;

Properties::Properties() = default;

void Properties::serialize(binn* document) {
    binn* vertices = binn_list();
    for (auto& v : vertexPropsList) {
        binn* prop = binn_object();
        v.second.serialize(prop);
        binn_list_add_object(vertices, prop);
        binn_free(prop);
    }
    binn* edges = binn_list();
    for (auto& e : edgePropsList) {
        binn* prop = binn_object();
        e.second.serialize(prop);
        binn_list_add_object(edges, prop);
        binn_free(prop);
    }
    binn_object_set_list(document, "vProps", vertices);
    binn_free(vertices);
    binn_object_set_list(document, "eProps", edges);
    binn_free(edges);
}

void Properties::deserialize(void* document) {
    vertexPropsList.clear();
    edgePropsList.clear();
    void* vertices = binn_object_list(document, "vProps");
    auto vCount = binn_count(vertices);
    for (int i = 1; i <= vCount; i++) {
        VertexProps props;
        void* buffer = binn_list_object(vertices, i);
        props.deserialize(buffer);
        vertexPropsList.insert({binn_object_str(buffer, "uuid"), props});
    }
    void* edges = binn_object_list(document, "eProps");
    auto eCount = binn_count(edges);
    for (int i = 1; i <= eCount; i++) {
        EdgeProps props;
        void* buffer = binn_list_object(edges, i);
        props.deserialize(buffer);
        edgePropsList.insert({binn_object_str(buffer, "uuid"), props});
    }
}

bool Properties::assignAccessName(const std::string& accessName, const gfn::Uuid& uuid, bool overwrite) {
    if (!isUuid(uuid)) {
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

gfn::Uuid Properties::convertAccessName(const std::string& accessName) {
    auto f = accessNames.find(accessName);
    if (f != accessNames.end())
        return f->second;
    else {
        return "";
        // access name is not assigned
    }
}

gfn::VertexProps* Properties::getVertexProps(const gfn::Uuid& uuid, bool warnOnNotFound) {
    auto it = vertexPropsList.find(uuid);
    if (it == vertexPropsList.end()) {
        if (warnOnNotFound) {
        }
        return nullptr;
    }
    return &it->second;
}

gfn::EdgeProps* Properties::getEdgeProps(const gfn::Uuid& edgeUuid, bool warnOnNotFound) {
    auto it = edgePropsList.find(edgeUuid);
    if (it == edgePropsList.end()) {
        if (warnOnNotFound) {
        }
        return nullptr;
    }
    return &it->second;
}

bool Properties::newVertexProps(const gfn::Uuid& uuid, bool clearExisting) {
    if (clearExisting && vertexPropsList.erase(uuid)) {
    }

    auto result = vertexPropsList.insert({uuid, gfn::VertexProps()});
    if (result.second) {
        std::uniform_real_distribution dis(-20.0, 20.0);
        result.first->second.position.value = gfn::Vec2(dis(gfn::getRng()),
                                                         dis(gfn::getRng()));
        getVertexProps(uuid)->uuid.value = uuid;
        return true;
    }
    return false;
}

bool Properties::newEdgeProps(const gfn::Uuid& edgeUuid, bool clearExisting) {
    if (clearExisting && edgePropsList.erase(edgeUuid)) {
    }

    auto result = edgePropsList.insert({edgeUuid, gfn::EdgeProps()});
    if (result.second) {
        result.first->second.edgeUuid.value = edgeUuid;
        return true;
    }
    return false;
}

bool Properties::eraseVertexProps(const gfn::Uuid& edgeUuid) {
    auto it = vertexPropsList.find(edgeUuid);
    if (it == vertexPropsList.end()) {
        return false;
    }
    vertexPropsList.erase(it);
    return true;
}

bool Properties::eraseEdgeProps(const gfn::Uuid& edgeUuid) {
    auto it = edgePropsList.find(edgeUuid);
    if (it == edgePropsList.end()) {
        return false;
    }
    edgePropsList.erase(it);
    return true;
}

std::unordered_map<gfn::Uuid, gfn::VertexProps>& Properties::getVertexPropsList() {
    return vertexPropsList;
}

std::unordered_map<gfn::Uuid, gfn::EdgeProps>& Properties::getEdgePropsList() { return edgePropsList; }


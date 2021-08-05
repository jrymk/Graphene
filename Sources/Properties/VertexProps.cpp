#include "VertexProps.h"

#include <binn.h>

namespace gfn {
    VertexProps::VertexProps() :
            uuid("uuid"),
            enabled("enabled", false),
            position("position"),
            vertexFillColor("vertexFillColor", IM_COL32(255, 211, 0, 255)),
            radius("radius", 0.5),
            force("force", gfn::Vec2(0.0, 0.0)),
            pauseUpdate("pauseUpdate", false),
            component("component", ""),
            isComponentRoot("isComponentRoot", false) {
    }

    void VertexProps::serialize(binn* object) {
        binn_object_set_str(object, uuid.key.c_str(), uuid.value.data());
        binn_object_set_bool(object, enabled.key.c_str(), enabled.value);
        binn_object_set_double(object, (position.key + ".x").c_str(), position.value.x);
        binn_object_set_double(object, (position.key + ".y").c_str(), position.value.y);
        binn_object_set_int32(object, vertexFillColor.key.c_str(), vertexFillColor.value);
        binn_object_set_double(object, radius.key.c_str(), radius.value);
        binn_object_set_double(object, (force.key + ".x").c_str(), force.value.x);
        binn_object_set_double(object, (force.key + ".y").c_str(), force.value.y);
        binn_object_set_bool(object, pauseUpdate.key.c_str(), pauseUpdate.value);
        binn_object_set_str(object, component.key.c_str(), component.value.data());
        binn_object_set_bool(object, isComponentRoot.key.c_str(), isComponentRoot.value);
    }

    void VertexProps::deserialize(void* object) {
        uuid.value = binn_object_str(object, uuid.key.c_str());
        enabled.value = binn_object_bool(object, enabled.key.c_str());
        position.value.x = binn_object_double(object, (position.key + ".x").c_str());
        position.value.y = binn_object_double(object, (position.key + ".y").c_str());
        vertexFillColor.value = binn_object_int32(object, vertexFillColor.key.c_str());
        radius.value = binn_object_double(object, radius.key.c_str());
        force.value.x = binn_object_double(object, (force.key + ".x").c_str());
        force.value.y = binn_object_double(object, (force.key + ".y").c_str());
        pauseUpdate.value = binn_object_bool(object, pauseUpdate.key.c_str());
        component.value = binn_object_str(object, component.key.c_str());
        isComponentRoot.value = binn_object_bool(object, isComponentRoot.key.c_str());
    }

    void VertexProps::get(const std::string& key, gfn::Command& output) {
        if (key == uuid.key) uuid.getValueStr(output);
        else if (key == enabled.key) enabled.getValueStr(output);
        else if (key == position.key) position.getValueStr(output);
        else if (key == vertexFillColor.key) vertexFillColor.getValueStr(output);
        else if (key == radius.key) radius.getValueStr(output);
        else if (key == force.key) force.getValueStr(output);
        else if (key == pauseUpdate.key) pauseUpdate.getValueStr(output);
        else if (key == component.key) component.getValueStr(output);
        else if (key == isComponentRoot.key) isComponentRoot.getValueStr(output);
    }

    void VertexProps::set(const std::string& key, const std::string& value, gfn::Command& output) {
        if (key == uuid.key) uuid.setValueStr(value, output);
        else if (key == enabled.key) enabled.setValueStr(value, output);
        else if (key == position.key) position.setValueStr(value, output);
        else if (key == vertexFillColor.key) vertexFillColor.setValueStr(value, output);
        else if (key == radius.key) radius.setValueStr(value, output);
        else if (key == force.key) force.setValueStr(value, output);
        else if (key == pauseUpdate.key) pauseUpdate.setValueStr(value, output);
        else if (key == component.key) component.setValueStr(value, output);
        else if (key == isComponentRoot.key) isComponentRoot.setValueStr(value, output);
    }
}
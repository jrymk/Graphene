#include "VertexProps.h"

#include <binn.h>

namespace gfn {
    VertexProps::VertexProps() :
            uuid("uuid"),
            label("label"),
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
        binn_object_set_str(object, label.key.c_str(), label.value.data());
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
        binn_iter iter;
        binn value;
        char key[256];
        binn_object_foreach(object, key, value) {
            if (key == uuid.key && value.type == BINN_STRING) uuid.get() = (char*) value.ptr;
            else if (key == label.key && value.type == BINN_STRING) label.get() = (char*) value.ptr;
            else if (key == enabled.key && value.type == BINN_BOOL) enabled.get() = value.vbool;
            else if (key == (position.key + ".x") && value.type == BINN_DOUBLE) position.get().x = value.vdouble;
            else if (key == (position.key + ".y") && value.type == BINN_DOUBLE) position.get().y = value.vdouble;
            else if (key == vertexFillColor.key && value.type == BINN_INT32) vertexFillColor.get() = value.vint32;
            else if (key == radius.key && value.type == BINN_DOUBLE) radius.get() = value.vdouble;
            else if (key == (force.key + ".x") && value.type == BINN_DOUBLE) force.get().x = value.vdouble;
            else if (key == (force.key + ".y") && value.type == BINN_DOUBLE) force.get().y = value.vdouble;
            else if (key == pauseUpdate.key && value.type == BINN_BOOL) pauseUpdate.get() = value.vbool;
            else if (key == component.key && value.type == BINN_STRING) component.get() = (char*) value.ptr;
            else if (key == isComponentRoot.key && value.type == BINN_BOOL) isComponentRoot.get() = value.vbool;
        }
    }

    void VertexProps::serializeJson(nlohmann::json& j) {
        uuid.serialize(j);
        label.serialize(j);
        enabled.serialize(j);
        position.serialize(j);
        vertexFillColor.serialize(j);
        radius.serialize(j);
        force.serialize(j);
        pauseUpdate.serialize(j);
        component.serialize(j);
        isComponentRoot.serialize(j);
    }

    void VertexProps::deserializeJson(nlohmann::json& j) {
        uuid.deserialize(j);
        label.deserialize(j);
        enabled.deserialize(j);
        position.deserialize(j);
        vertexFillColor.deserialize(j);
        radius.deserialize(j);
        force.deserialize(j);
        pauseUpdate.deserialize(j);
        component.deserialize(j);
        isComponentRoot.deserialize(j);
    }

    void VertexProps::get(const std::string& key, gfn::Args& output) {
        if (key == uuid.key) uuid.getValueStr(output);
        else if (key == label.key) label.getValueStr(output);
        else if (key == enabled.key) enabled.getValueStr(output);
        else if (key == position.key) position.getValueStr(output);
        else if (key == vertexFillColor.key) vertexFillColor.getValueStr(output);
        else if (key == radius.key) radius.getValueStr(output);
        else if (key == force.key) force.getValueStr(output);
        else if (key == pauseUpdate.key) pauseUpdate.getValueStr(output);
        else if (key == component.key) component.getValueStr(output);
        else if (key == isComponentRoot.key) isComponentRoot.getValueStr(output);
    }

    void VertexProps::set(const std::string& key, const std::string& value, gfn::Args& output) {
        if (key == uuid.key) uuid.setValueStr(value, output);
        else if (key == label.key) label.setValueStr(value, output);
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
#include "EdgeProps.h"

#include <binn.h>

namespace gfn {
    EdgeProps::EdgeProps() :
            edgeUuid("uuid"),
            enabled("enabled", false),
            position("position"),
            startVertexUuid("startVertexUuid"),
            startVertexPosition("endVertexPosition"),
            endVertexUuid("endVertexUuid"),
            endVertexPosition("endVertexPosition"),
            edgeFillColor("edgeFillColor", IM_COL32(0, 0, 0, 255)),
            thickness("thickness", 0.3) {}

    void EdgeProps::serialize(binn* object) {
        binn_object_set_str(object, edgeUuid.key.c_str(), edgeUuid.value.data());
        binn_object_set_bool(object, enabled.key.c_str(), enabled.value);
        binn_object_set_double(object, (position.key + ".x").c_str(), position.value.x);
        binn_object_set_double(object, (position.key + ".y").c_str(), position.value.y);
        binn_object_set_str(object, startVertexUuid.key.c_str(), startVertexUuid.value.data());
        binn_object_set_double(object, (startVertexPosition.key + ".x").c_str(), startVertexPosition.value.x);
        binn_object_set_double(object, (startVertexPosition.key + ".y").c_str(), startVertexPosition.value.y);
        binn_object_set_str(object, endVertexUuid.key.c_str(), endVertexUuid.value.data());
        binn_object_set_double(object, (endVertexPosition.key + ".x").c_str(), endVertexPosition.value.x);
        binn_object_set_double(object, (endVertexPosition.key + ".y").c_str(), endVertexPosition.value.y);
        binn_object_set_int32(object, edgeFillColor.key.c_str(), edgeFillColor.value);
        binn_object_set_double(object, thickness.key.c_str(), thickness.value);
    }

    void EdgeProps::deserialize(void* object) {
        edgeUuid.value = binn_object_str(object, edgeUuid.key.c_str());
        enabled.value = binn_object_bool(object, enabled.key.c_str());
        position.value.x = binn_object_double(object, (position.key + ".x").c_str());
        position.value.y = binn_object_double(object, (position.key + ".y").c_str());
        startVertexUuid.value = binn_object_str(object, startVertexUuid.key.c_str());
        startVertexPosition.value.x = binn_object_double(object, (startVertexPosition.key + ".x").c_str());
        startVertexPosition.value.y = binn_object_double(object, (startVertexPosition.key + ".y").c_str());
        endVertexUuid.value = binn_object_str(object, endVertexUuid.key.c_str());
        endVertexPosition.value.x = binn_object_double(object, (endVertexPosition.key + ".x").c_str());
        endVertexPosition.value.y = binn_object_double(object, (endVertexPosition.key + ".y").c_str());
        edgeFillColor.value = binn_object_int32(object, edgeFillColor.key.c_str());
        thickness.value = binn_object_double(object, thickness.key.c_str());
    }

    void EdgeProps::get(const std::string& key, gfn::Args& output) {
        if (key == edgeUuid.key) edgeUuid.getValueStr(output);
        else if (key == enabled.key) enabled.getValueStr(output);
        else if (key == position.key) position.getValueStr(output);
        else if (key == startVertexUuid.key) startVertexUuid.getValueStr(output);
        else if (key == startVertexPosition.key) startVertexPosition.getValueStr(output);
        else if (key == endVertexUuid.key) endVertexUuid.getValueStr(output);
        else if (key == endVertexPosition.key) endVertexPosition.getValueStr(output);
        else if (key == edgeFillColor.key) edgeFillColor.getValueStr(output);
        else if (key == thickness.key) thickness.getValueStr(output);
    }

    void EdgeProps::set(const std::string& key, const std::string& value, gfn::Args& output) {
        if (key == edgeUuid.key) edgeUuid.setValueStr(value, output);
        else if (key == enabled.key) enabled.setValueStr(value, output);
        else if (key == position.key) position.setValueStr(value, output);
        else if (key == startVertexUuid.key) startVertexUuid.setValueStr(value, output);
        else if (key == startVertexPosition.key) startVertexPosition.setValueStr(value, output);
        else if (key == endVertexUuid.key) endVertexUuid.setValueStr(value, output);
        else if (key == endVertexPosition.key) endVertexPosition.setValueStr(value, output);
        else if (key == edgeFillColor.key) edgeFillColor.setValueStr(value, output);
        else if (key == thickness.key) thickness.setValueStr(value, output);
    }
}
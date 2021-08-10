#include "EdgeProps.h"

#include <binn.h>

namespace gfn {
    EdgeProps::EdgeProps() :
            edgeUuid("uuid"),
            label("label"),
            labelSize("labelSize", 0.5),
            labelColor("labelColor", IM_COL32(0, 0, 0, 255)),
            enabled("enabled", false),
            position("position"),
            startVertexUuid("startVertexUuid"),
            startVertexPosition("endVertexPosition"),
            endVertexUuid("endVertexUuid"),
            endVertexPosition("endVertexPosition"),
            edgeColor("edgeColor", IM_COL32(0, 0, 0, 255)),
            thickness("thickness", 0.06) {}

    void EdgeProps::serialize(binn* object) {
        binn_object_set_str(object, edgeUuid.key.c_str(), edgeUuid.value.data());
        binn_object_set_str(object, label.key.c_str(), label.value.data());
        binn_object_set_double(object, labelSize.key.c_str(), labelSize.value);
        binn_object_set_int32(object, labelColor.key.c_str(), labelColor.value);
        binn_object_set_bool(object, enabled.key.c_str(), enabled.value);
        binn_object_set_double(object, (position.key + ".x").c_str(), position.value.x);
        binn_object_set_double(object, (position.key + ".y").c_str(), position.value.y);
        binn_object_set_str(object, startVertexUuid.key.c_str(), startVertexUuid.value.data());
        binn_object_set_double(object, (startVertexPosition.key + ".x").c_str(), startVertexPosition.value.x);
        binn_object_set_double(object, (startVertexPosition.key + ".y").c_str(), startVertexPosition.value.y);
        binn_object_set_str(object, endVertexUuid.key.c_str(), endVertexUuid.value.data());
        binn_object_set_double(object, (endVertexPosition.key + ".x").c_str(), endVertexPosition.value.x);
        binn_object_set_double(object, (endVertexPosition.key + ".y").c_str(), endVertexPosition.value.y);
        binn_object_set_int32(object, edgeColor.key.c_str(), edgeColor.value);
        binn_object_set_double(object, thickness.key.c_str(), thickness.value);
    }

    void EdgeProps::deserialize(void* object) {
        binn_iter iter;
        binn value;
        char key[256];
        binn_object_foreach(object, key, value) {
            if (key == edgeUuid.key && value.type == BINN_STRING) edgeUuid.get() = (char*) value.ptr;
            else if (key == label.key && value.type == BINN_STRING) label.get() = (char*) value.ptr;
            else if (key == labelSize.key && value.type == BINN_DOUBLE) labelSize.get() = value.vdouble;
            else if (key == labelColor.key && value.type == BINN_INT32) labelColor.get() = value.vint32;
            else if (key == enabled.key && value.type == BINN_BOOL) enabled.get() = value.vbool;
            else if (key == (position.key + ".x") && value.type == BINN_DOUBLE) position.get().x = value.vdouble;
            else if (key == (position.key + ".y") && value.type == BINN_DOUBLE) position.get().y = value.vdouble;
            else if (key == startVertexUuid.key && value.type == BINN_STRING) startVertexUuid.get() = (char*) value.ptr;
            else if (key == (startVertexPosition.key + ".x") && value.type == BINN_DOUBLE) startVertexPosition.get().x = value.vdouble;
            else if (key == (startVertexPosition.key + ".y") && value.type == BINN_DOUBLE) startVertexPosition.get().y = value.vdouble;
            else if (key == endVertexUuid.key && value.type == BINN_STRING) endVertexUuid.get() = (char*) value.ptr;
            else if (key == (endVertexPosition.key + ".x") && value.type == BINN_DOUBLE) endVertexPosition.get().x = value.vdouble;
            else if (key == (endVertexPosition.key + ".y") && value.type == BINN_DOUBLE) endVertexPosition.get().y = value.vdouble;
            else if (key == edgeColor.key && value.type == BINN_INT32) edgeColor.get() = value.vint32;
            else if (key == thickness.key && value.type == BINN_DOUBLE) thickness.get() = value.vdouble;
        }
    }

    void EdgeProps::serializeJson(nlohmann::json& j) {
        edgeUuid.serialize(j);
        enabled.serialize(j);
        label.serialize(j);
        labelSize.serialize(j);
        labelColor.serialize(j);
        position.serialize(j);
        startVertexUuid.serialize(j);
        startVertexPosition.serialize(j);
        endVertexUuid.serialize(j);
        endVertexPosition.serialize(j);
        edgeColor.serialize(j);
        thickness.serialize(j);
    }

    void EdgeProps::deserializeJson(nlohmann::json& j) {
        edgeUuid.deserialize(j);
        label.deserialize(j);
        labelSize.deserialize(j);
        labelColor.deserialize(j);
        enabled.deserialize(j);
        position.deserialize(j);
        startVertexUuid.deserialize(j);
        startVertexPosition.deserialize(j);
        endVertexUuid.deserialize(j);
        endVertexPosition.deserialize(j);
        edgeColor.deserialize(j);
        thickness.deserialize(j);
    }

    void EdgeProps::get(const std::string& key, gfn::Args& output) {
        if (key == edgeUuid.key) edgeUuid.getValueStr(output);
        else if (key == label.key) label.getValueStr(output);
        else if (key == labelSize.key) labelSize.getValueStr(output);
        else if (key == labelColor.key) labelColor.getValueStr(output);
        else if (key == enabled.key) enabled.getValueStr(output);
        else if (key == position.key) position.getValueStr(output);
        else if (key == startVertexUuid.key) startVertexUuid.getValueStr(output);
        else if (key == startVertexPosition.key) startVertexPosition.getValueStr(output);
        else if (key == endVertexUuid.key) endVertexUuid.getValueStr(output);
        else if (key == endVertexPosition.key) endVertexPosition.getValueStr(output);
        else if (key == edgeColor.key) edgeColor.getValueStr(output);
        else if (key == thickness.key) thickness.getValueStr(output);
    }

    void EdgeProps::set(const std::string& key, const std::string& value, gfn::Args& output) {
        if (key == edgeUuid.key) edgeUuid.setValueStr(value, output);
        else if (key == label.key) label.setValueStr(value, output);
        else if (key == labelSize.key) labelSize.setValueStr(value, output);
        else if (key == labelColor.key) labelColor.setValueStr(value, output);
        else if (key == enabled.key) enabled.setValueStr(value, output);
        else if (key == position.key) position.setValueStr(value, output);
        else if (key == startVertexUuid.key) startVertexUuid.setValueStr(value, output);
        else if (key == startVertexPosition.key) startVertexPosition.setValueStr(value, output);
        else if (key == endVertexUuid.key) endVertexUuid.setValueStr(value, output);
        else if (key == endVertexPosition.key) endVertexPosition.setValueStr(value, output);
        else if (key == edgeColor.key) edgeColor.setValueStr(value, output);
        else if (key == thickness.key) thickness.setValueStr(value, output);
    }
}
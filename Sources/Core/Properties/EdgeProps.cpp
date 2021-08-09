#include "EdgeProps.h"

#include <binn.h>

namespace gfn {
    EdgeProps::EdgeProps() :
            edgeUuid("uuid"),
            label("label"),
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
        binn_object_set_str(object, label.key.c_str(), label.value.data());
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
        binn_iter iter;
        binn value;
        char key[256];
        binn_object_foreach(object, key, value) {
            if (key == edgeUuid.key && value.type == BINN_STRING) edgeUuid.get() = (char*) value.ptr;
            else if (key == label.key && value.type == BINN_STRING) label.get() = (char*) value.ptr;
            else if (key == enabled.key && value.type == BINN_BOOL) enabled.get() = value.vbool;
            else if (key == (position.key + ".x") && value.type == BINN_DOUBLE) position.get().x = value.vdouble;
            else if (key == (position.key + ".y") && value.type == BINN_DOUBLE) position.get().y = value.vdouble;
            else if (key == startVertexUuid.key && value.type == BINN_STRING) startVertexUuid.get() = (char*) value.ptr;
            else if (key == (startVertexPosition.key + ".x") && value.type == BINN_DOUBLE) startVertexPosition.get().x = value.vdouble;
            else if (key == (startVertexPosition.key + ".y") && value.type == BINN_DOUBLE) startVertexPosition.get().y = value.vdouble;
            else if (key == endVertexUuid.key && value.type == BINN_STRING) endVertexUuid.get() = (char*) value.ptr;
            else if (key == (endVertexPosition.key + ".x") && value.type == BINN_DOUBLE) endVertexPosition.get().x = value.vdouble;
            else if (key == (endVertexPosition.key + ".y") && value.type == BINN_DOUBLE) endVertexPosition.get().y = value.vdouble;
            else if (key == edgeFillColor.key && value.type == BINN_INT32) edgeFillColor.get() = value.vint32;
            else if (key == thickness.key && value.type == BINN_DOUBLE) thickness.get() = value.vdouble;
        }
    }

    void EdgeProps::get(const std::string& key, gfn::Args& output) {
        if (key == edgeUuid.key) edgeUuid.getValueStr(output);
        else if (key == label.key) label.getValueStr(output);
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
        else if (key == label.key) label.setValueStr(value, output);
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
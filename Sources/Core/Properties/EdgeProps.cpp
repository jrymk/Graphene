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
            thickness("thickness", 0.06),
            arrowStyle("arrowStyle", 0),
            force("force", gfn::Vec2(0.0, 0.0)),
            pauseUpdate("pauseUpdate", false) {}

    void EdgeProps::serialize(binn* object) {
        binn_object_set_str(object, edgeUuid.key.c_str(), edgeUuid.value.data());
        binn_object_set_str(object, label.key.c_str(), label.value.data());
        binn_object_set_double(object, labelSize.key.c_str(), labelSize.value);
        binn_object_set_uint32(object, labelColor.key.c_str(), labelColor.value);
        binn_object_set_bool(object, enabled.key.c_str(), enabled.value);
        binn_object_set_double(object, (position.key + ".x").c_str(), position.value.x);
        binn_object_set_double(object, (position.key + ".y").c_str(), position.value.y);
        binn_object_set_str(object, startVertexUuid.key.c_str(), startVertexUuid.value.data());
        binn_object_set_double(object, (startVertexPosition.key + ".x").c_str(), startVertexPosition.value.x);
        binn_object_set_double(object, (startVertexPosition.key + ".y").c_str(), startVertexPosition.value.y);
        binn_object_set_str(object, endVertexUuid.key.c_str(), endVertexUuid.value.data());
        binn_object_set_double(object, (endVertexPosition.key + ".x").c_str(), endVertexPosition.value.x);
        binn_object_set_double(object, (endVertexPosition.key + ".y").c_str(), endVertexPosition.value.y);
        binn_object_set_uint32(object, edgeColor.key.c_str(), edgeColor.value);
        binn_object_set_double(object, thickness.key.c_str(), thickness.value);
        binn_object_set_int32(object, arrowStyle.key.c_str(), arrowStyle.value);
        binn_object_set_double(object, (force.key + ".x").c_str(), force.value.x);
        binn_object_set_double(object, (force.key + ".y").c_str(), force.value.y);
        binn_object_set_bool(object, pauseUpdate.key.c_str(), pauseUpdate.value);
    }

    void EdgeProps::deserialize(void* object) {
        binn_iter iter;
        binn value;
        char key[256];
        binn_object_foreach(object, key, value) {
            if (key == edgeUuid.key && value.type == BINN_STRING) edgeUuid.value = (char*) value.ptr;
            else if (key == label.key && value.type == BINN_STRING) label.value = (char*) value.ptr;
            else if (key == labelSize.key && value.type == BINN_DOUBLE) labelSize.value = value.vdouble;
            else if (key == labelColor.key && value.type == BINN_UINT32) labelColor.value = value.vuint32;
            else if (key == enabled.key && value.type == BINN_BOOL) enabled.value = value.vbool;
            else if (key == (position.key + ".x") && value.type == BINN_DOUBLE) position.value.x = value.vdouble;
            else if (key == (position.key + ".y") && value.type == BINN_DOUBLE) position.value.y = value.vdouble;
            else if (key == startVertexUuid.key && value.type == BINN_STRING) startVertexUuid.value = (char*) value.ptr;
            else if (key == (startVertexPosition.key + ".x") && value.type == BINN_DOUBLE) startVertexPosition.value.x = value.vdouble;
            else if (key == (startVertexPosition.key + ".y") && value.type == BINN_DOUBLE) startVertexPosition.value.y = value.vdouble;
            else if (key == endVertexUuid.key && value.type == BINN_STRING) endVertexUuid.value = (char*) value.ptr;
            else if (key == (endVertexPosition.key + ".x") && value.type == BINN_DOUBLE) endVertexPosition.value.x = value.vdouble;
            else if (key == (endVertexPosition.key + ".y") && value.type == BINN_DOUBLE) endVertexPosition.value.y = value.vdouble;
            else if (key == edgeColor.key && value.type == BINN_UINT32) edgeColor.value = value.vuint32;
            else if (key == thickness.key && value.type == BINN_DOUBLE) thickness.value = value.vdouble;
            else if (key == arrowStyle.key && value.type == BINN_INT32) arrowStyle.value = value.vint32;
            else if (key == (force.key + ".x") && value.type == BINN_DOUBLE) force.value.x = value.vdouble;
            else if (key == (force.key + ".y") && value.type == BINN_DOUBLE) force.value.y = value.vdouble;
            else if (key == pauseUpdate.key && value.type == BINN_BOOL) pauseUpdate.value = value.vbool;
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
        arrowStyle.serialize(j);
        force.serialize(j);
        pauseUpdate.serialize(j);
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
        arrowStyle.deserialize(j);
        force.deserialize(j);
        pauseUpdate.deserialize(j);
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
        else if (key == arrowStyle.key) arrowStyle.getValueStr(output);
        else if (key == force.key) force.getValueStr(output);
        else if (key == pauseUpdate.key) pauseUpdate.getValueStr(output);
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
        else if (key == arrowStyle.key) arrowStyle.setValueStr(value, output);
        else if (key == force.key) force.setValueStr(value, output);
        else if (key == pauseUpdate.key) pauseUpdate.setValueStr(value, output);
    }
}
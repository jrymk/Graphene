#pragma once

#include <Properties/Properties.hpp>
#include <Parser/Parser.hpp>

namespace gfn::properties {
    void setVertexProp(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        std::pair<gfn::properties::VertexProps*, gfn::properties::VertexPropsInternal*> p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = properties->getVertexProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = properties->convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = properties->getVertexProps(uuid);
        if (p.first == nullptr || p.second == nullptr) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "VERTEX_PROP_NOT_FOUND");
            return;
        }
        auto key = command.getParamValue("-key");
        auto value = command.getParamValue("-value");
        if (key.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "KEY_UNSPECIFIED");
            output.newParam("-fix", "Specify key with [-key]");
            return;
        }
        if (value.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "VALUE_UNSPECIFIED");
            output.newParam("-fix", "Specify value with [-value]");
            return;
        }

        if (key == "uuid") {
            output.newParam("-error", "UUID_READ_ONLY");
            output.newParam("-fix", "Uuid can not be modified. Add new vertex with specified uuid instead");
            return;
        }
        if (key == "enabled") {
            parser::parseBool(p.first->enabled, value, output);
            return;
        }
        if (key == "position.x") {
            parser::parseFloat(p.first->position.x, value, output);
            return;
        }
        if (key == "position.y") {
            parser::parseFloat(p.first->position.y, value, output);
            return;
        }
        if (key == "vertexFillColor") {
            int color32;
            parser::parseInt(color32, value, output);
            p.first->vertexFillColor.color32 = color32;
            if (output.getParamValue("-parse-successful") == "true")
                return;
            output.newParam("-fix",
                            "Expected 32 bit integer, do 16777216r + 65536g + 256b + a or use ImGui::ColorConvertFloat4ToU32()");
            return;
        }
        if (key == "radius") {
            parser::parseFloat(p.first->radius, value, output);
            return;
        }
    }

    void getVertexProp(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        std::pair<gfn::properties::VertexProps*, gfn::properties::VertexPropsInternal*> p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = properties->getVertexProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = properties->convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = properties->getVertexProps(uuid);
        if (p.first == nullptr || p.second == nullptr) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "VERTEX_PROP_NOT_FOUND");
            return;
        }
        auto key = command.getParamValue("-key");
        if (key.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "KEY_UNSPECIFIED");
            output.newParam("-fix", "Specify key with [-key]");
            return;
        }

        if (key == "uuid") {
            output.newParam("-value", p.first->uuid);
            return;
        }
        if (key == "enabled") {
            output.newParam("-value", p.first->enabled ? "true" : "false");
            return;
        }
        if (key == "position.x") {
            output.newParam("-value", std::to_string(p.first->position.x));
            return;
        }
        if (key == "position.y") {
            output.newParam("-value", std::to_string(p.first->position.y));
            return;
        }
        if (key == "vertexFillColor") {
            output.newParam("-value", std::to_string(p.first->vertexFillColor.color32));
            return;
        }
        if (key == "radius") {
            output.newParam("-value", std::to_string(p.first->radius));
            return;
        }
    }

    void setEdgeProp(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        std::pair<gfn::properties::EdgeProps*, gfn::properties::EdgePropsInternal*> p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = properties->getEdgeProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = properties->convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = properties->getEdgeProps(uuid);
        if (p.first == nullptr || p.second == nullptr) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "EDGE_PROP_NOT_FOUND");
            return;
        }
        auto key = command.getParamValue("-key");
        auto value = command.getParamValue("-value");
        if (key.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "KEY_UNSPECIFIED");
            output.newParam("-fix", "Specify key with [-key]");
            return;
        }
        if (value.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "VALUE_UNSPECIFIED");
            output.newParam("-fix", "Specify value with [-value]");
            return;
        }

        if (key == "edgeUuid") {
            output.newParam("-error", "UUID_READ_ONLY");
            output.newParam("-fix", "Uuid can not be modified. Add new vertex with specified uuid instead");
            return;
        }
        if (key == "enabled") {
            parser::parseBool(p.first->enabled, value, output);
            return;
        }
        if (key == "startVertexUuid") {
            if (properties->getVertexProps(value).first)
                p.first->startVertexUuid = value;
            else {
                output.newParam("-error", "VERTEX_NOT_FOUND");
                output.newParam("-fix", "Expected a vertex uuid");
            }
            return;
        }
        if (key == "endVertexUuid") {
            if (properties->getVertexProps(value).first)
                p.first->endVertexUuid = value;
            else {
                output.newParam("-error", "VERTEX_NOT_FOUND");
                output.newParam("-fix", "Expected a vertex uuid");
            }
            return;
        }
        if (key == "edgeFillColor") {
            int color32;
            parser::parseInt(color32, value, output);
            p.first->edgeFillColor.color32 = color32;
            if (output.getParamValue("-parse-successful") == "true")
                return;
            output.newParam("-fix",
                            "Expected 32 bit integer, do 16777216r + 65536g + 256b + a or use ImGui::ColorConvertFloat4ToU32()");
            return;
        }
        if (key == "thickness") {
            parser::parseFloat(p.first->thickness, value, output);
            return;
        }
    }

    void getEdgeProp(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        std::pair<gfn::properties::EdgeProps*, gfn::properties::EdgePropsInternal*> p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = properties->getEdgeProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = properties->convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = properties->getEdgeProps(uuid);
        if (p.first == nullptr || p.second == nullptr) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "EDGE_PROP_NOT_FOUND");
            return;
        }
        auto key = command.getParamValue("-key");
        if (key.empty()) {
            output.newParam("-uuid", uuid);
            output.newParam("-error", "KEY_UNSPECIFIED");
            output.newParam("-fix", "Specify key with [-key]");
            return;
        }

        if (key == "edgeUuid") {
            output.newParam("-value", p.first->edgeUuid);
            return;
        }if (key == "startVertexUuid") {
            output.newParam("-value", p.first->startVertexUuid);
            return;
        }if (key == "endVertexUuid") {
            output.newParam("-value", p.first->endVertexUuid);
            return;
        }
        if (key == "edgeFillColor") {
            output.newParam("-value", std::to_string(p.first->edgeFillColor.color32));
            return;
        }
        if (key == "thickness") {
            output.newParam("-value", std::to_string(p.first->thickness));
            return;
        }
    }

    bool tryParse(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        auto cmd = command.getParamValue("command");
        if (cmd == "setvertexprop") {
            setVertexProp(properties, command, output);
            return true;
        } else if (cmd == "getvertexprop") {
            getVertexProp(properties, command, output);
            return true;
        } else if (cmd == "setedgeprop") {
            setEdgeProp(properties, command, output);
            return true;
        } else if (cmd == "getedgeprop") {
            getEdgeProp(properties, command, output);
            return true;
        }
        return false;
    }
}
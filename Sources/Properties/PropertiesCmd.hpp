#pragma once

#include <Properties/Properties.hpp>

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
            if (value == "true")
                p.first->enabled = true;
            else if (value == "false")
                p.first->enabled = false;
            else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", R"(Expected "true" or "false")");
                return;
            }
        }
        if (key == "position") {
            double x, y;
            if (sscanf(value.c_str(), "(%lf, %lf)", &x, &y) == 2) {
                p.first->position.x = x;
                p.first->position.y = y;
            } else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected (x, y)");
            }
            return;
        }
        if (key == "position.x") {
            double x;
            if (sscanf(value.c_str(), "%lf", &x) == 1) {
                p.first->position.x = x;
            } else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected a floating point number");
            }
            return;
        }
        if (key == "position.y") {
            double y;
            if (sscanf(value.c_str(), "%lf", &y) == 1) {
                p.first->position.y = y;
            } else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected a floating point number");
            }
            return;
        }
        if (key == "vertexFillColor") {
            int r, g, b, a;
            if (sscanf(value.c_str(), "(%d, %d, %d, %d)", &r, &g, &b, &a) == 4) {
                if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255 && a >= 0 && a <= 255)
                    p.first->vertexFillColor.setRGBA(r, g, b, a);
                else {
                    output.newParam("-error", "ILLEGAL_VALUE");
                    output.newParam("-fix", "Expected unsigned integers within range of 0 to 255");
                }
            } else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected (r, g, b, a)");
            }
            return;
        }
        if (key == "radius") {
            double r;
            std::stringstream ss(value);
            if (ss >> r) {
                p.first->radius = r;
            } else {
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected a floating point number");
            }
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
        if (key == "position") {
            output.newParam("-value",
                            "(" + std::to_string(p.first->position.x) + ", " + std::to_string(p.first->position.y) +
                            ")");
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
            ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(p.first->vertexFillColor.color32);
            output.newParam("-value",
                            "(" + std::to_string(int(rgba.x * 255)) + ", " + std::to_string(int(rgba.y * 255)) + ", " +
                            std::to_string(int(rgba.z * 255)) + ", " + std::to_string(int(rgba.w * 255)) + ")");
            return;
        }
        if (key == "radius") {
            output.newParam("-value", std::to_string(p.first->radius));
            return;
        }
    }

    bool tryInterpret(gfn::properties::Properties* properties, gfn::Command command, gfn::Command& output) {
        auto cmd = command.getParamValue("command");
        if (cmd == "setvertexprop") {
            setVertexProp(properties, command, output);
            return true;
        } else if (cmd == "getvertexprop") {
            getVertexProp(properties, command, output);
            return true;
        } else if (cmd == "setedgeprop") {
            return true;
        } else if (cmd == "getedgeprop") {
            return true;
        }
        return false;
    }
}
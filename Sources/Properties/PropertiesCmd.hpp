#pragma once

#include <Properties/Properties.hpp>

namespace gfn::props {
    void setVertexProp(gfn::props::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        gfn::props::VertexProps* p;
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
        if (p == nullptr) {
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

        p->set(key, value, output);
    }

    void getVertexProp(gfn::props::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        gfn::props::VertexProps* p;
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
        if (p == nullptr || p == nullptr) {
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

        p->get(key, output);
    }

    void setEdgeProp(gfn::props::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        gfn::props::EdgeProps* p;
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
        if (p == nullptr || p == nullptr) {
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

        p->set(key, value, output);
    }

    void getEdgeProp(gfn::props::Properties* properties, gfn::Command command, gfn::Command& output) {
        gfn::Uuid uuid;
        gfn::props::EdgeProps* p;
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
        if (p == nullptr || p == nullptr) {
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

        p->get(key, output);
    }

    bool tryParse(gfn::props::Properties* properties, gfn::Command command, gfn::Command& output) {
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
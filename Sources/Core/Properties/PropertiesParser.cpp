#include "Properties.h"

namespace gfn {
    void Properties::setVertexProp(gfn::Args command, gfn::Args& output) {
        gfn::Uuid uuid;
        gfn::VertexProps* p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = getVertexProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = getVertexProps(uuid);
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

        p->set(key, value, output);
    }

    void Properties::getVertexProp(gfn::Args command, gfn::Args& output) {
        gfn::Uuid uuid;
        gfn::VertexProps* p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = getVertexProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = getVertexProps(uuid);
        if (p == nullptr) {
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

    void Properties::setEdgeProp(gfn::Args command, gfn::Args& output) {
        gfn::Uuid uuid;
        gfn::EdgeProps* p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = getEdgeProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = getEdgeProps(uuid);
        if (p == nullptr) {
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

        p->set(key, value, output);
    }

    void Properties::getEdgeProp(gfn::Args command, gfn::Args& output) {
        gfn::Uuid uuid;
        gfn::EdgeProps* p;
        if (!command.getParamValue("-uuid").empty()) {
            // uuid provided
            uuid = command.getParamValue("-uuid");
            p = getEdgeProps(uuid);

        } else if (!command.getParamValue("-name").empty()) {
            // name provided
            uuid = convertAccessName(command.getParamValue("-name"));
            if (uuid.empty()) {
                output.newParam("-error", "ACCESS_NAME_UNDEFINED");
                return;
            }
        }
        p = getEdgeProps(uuid);
        if (p == nullptr) {
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
}
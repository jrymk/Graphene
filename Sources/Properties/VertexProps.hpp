#pragma once

#include <Objects/Parsables.hpp>
#include <json.hpp>

namespace gfn::props {
    ///@brief Vertex properties that will be copied to the rendering thread / properties for the end user
    struct VertexProps {
        gfn::parsables::Uuid uuid;
        gfn::parsables::Bool enabled;
        gfn::parsables::Vec2f position;
        gfn::parsables::Color vertexFillColor;
        gfn::parsables::Double radius;
        gfn::parsables::Vec2f force;
        gfn::parsables::Vec2f componentCentroidPosition;
        gfn::parsables::Uuid component;
        gfn::parsables::Bool isComponentRoot;

        VertexProps() : uuid("uuid"),
                        enabled("enabled", false),
                        position("position"),
                        vertexFillColor("vertexFillColor", IM_COL32(255, 211, 0, 255)),
                        radius("radius", 0.5),
                        force("force", gfn::Vec2f(0.0, 0.0)),
                        componentCentroidPosition("componentCentroidPosition"),
                        component("component", ""),
                        isComponentRoot("isComponentRoot", false) {
        }

        void serialize(nlohmann::json& j) {
            uuid.serialize(j);
            enabled.serialize(j);
            position.serialize(j);
            vertexFillColor.serialize(j);
            radius.serialize(j);
            force.serialize(j);
            componentCentroidPosition.serialize(j);
            component.serialize(j);
            isComponentRoot.serialize(j);
        }

        void deserialize(nlohmann::json& j) {
            uuid.deserialize(j);
            enabled.deserialize(j);
            position.deserialize(j);
            vertexFillColor.deserialize(j);
            radius.deserialize(j);
            force.deserialize(j);
            componentCentroidPosition.deserialize(j);
            component.deserialize(j);
            isComponentRoot.deserialize(j);
        }

        void get(const std::string& key, gfn::Command& output) {
            if (key == uuid.key) uuid.getValueStr(output);
            else if (key == enabled.key) enabled.getValueStr(output);
            else if (key == position.key) position.getValueStr(output);
            else if (key == vertexFillColor.key) vertexFillColor.getValueStr(output);
            else if (key == radius.key) radius.getValueStr(output);
            else if (key == force.key) force.getValueStr(output);
            else if (key == componentCentroidPosition.key) componentCentroidPosition.getValueStr(output);
            else if (key == component.key) component.getValueStr(output);
            else if (key == isComponentRoot.key) isComponentRoot.getValueStr(output);
        }

        void set(const std::string& key, const std::string& value, gfn::Command& output) {
            if (key == uuid.key) uuid.setValueStr(value, output);
            else if (key == enabled.key) enabled.setValueStr(value, output);
            else if (key == position.key) position.setValueStr(value, output);
            else if (key == vertexFillColor.key) vertexFillColor.setValueStr(value, output);
            else if (key == radius.key) radius.setValueStr(value, output);
            else if (key == force.key) force.setValueStr(value, output);
            else if (key == componentCentroidPosition.key) componentCentroidPosition.setValueStr(value, output);
            else if (key == component.key) component.setValueStr(value, output);
            else if (key == isComponentRoot.key) isComponentRoot.setValueStr(value, output);
        }
    };
} // namespace gfn::core::properties
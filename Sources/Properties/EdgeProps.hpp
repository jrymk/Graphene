#pragma once

#include <Objects/Parsables.hpp>
#include <json.hpp>

namespace gfn::props {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::parsables::Uuid edgeUuid;
        gfn::parsables::Bool enabled;
        gfn::parsables::Vec2f position;
        gfn::parsables::Uuid startVertexUuid;
        gfn::parsables::Vec2f startVertexPosition;
        gfn::parsables::Uuid endVertexUuid;
        gfn::parsables::Vec2f endVertexPosition;
        gfn::parsables::Color edgeFillColor;
        gfn::parsables::Double thickness;

        EdgeProps() :
                edgeUuid("uuid"),
                enabled("enabled", false),
                position("position"),
                startVertexUuid("startVertexUuid"),
                startVertexPosition("endVertexPosition"),
                endVertexUuid("endVertexUuid"),
                endVertexPosition("endVertexPosition"),
                edgeFillColor("edgeFillColor", IM_COL32(0, 0, 0, 255)),
                thickness("thickness", 0.3) {
        }

        void serialize(nlohmann::json& j) {
            edgeUuid.serialize(j);
            enabled.serialize(j);
            position.serialize(j);
            startVertexUuid.serialize(j);
            startVertexPosition.serialize(j);
            endVertexUuid.serialize(j);
            endVertexPosition.serialize(j);
            edgeFillColor.serialize(j);
            thickness.serialize(j);
        }

        void deserialize(nlohmann::json& j) {
            edgeUuid.deserialize(j);
            enabled.deserialize(j);
            position.deserialize(j);
            startVertexUuid.deserialize(j);
            startVertexPosition.deserialize(j);
            endVertexUuid.deserialize(j);
            endVertexPosition.deserialize(j);
            edgeFillColor.deserialize(j);
            thickness.deserialize(j);
        }

        void get(const std::string& key, gfn::Command& output) {
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

        void set(const std::string& key, const std::string& value, gfn::Command& output) {
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


    };
} // namespace gfn::core::properties
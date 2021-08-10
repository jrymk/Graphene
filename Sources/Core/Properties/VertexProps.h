#ifndef VERTEXPROPS_H
#define VERTEXPROPS_H

#include <Core/Objects/Serializable.h>
#include <binn.h>

namespace gfn {
    ///@brief Vertex properties that will be copied to the rendering thread / properties for the end user
    struct VertexProps {
        gfn::serializable::Uuid uuid;
        gfn::serializable::String label;
        gfn::serializable::Double labelSize;
        gfn::serializable::Color labelColor;
        gfn::serializable::Bool enabled;
        gfn::serializable::Vec2f position;
        gfn::serializable::Color fillColor;
        gfn::serializable::Double radius;
        gfn::serializable::Color outlineColor;
        gfn::serializable::Double outlineThickness;
        gfn::serializable::Vec2f force;
        gfn::serializable::Bool pauseUpdate;
        gfn::serializable::Uuid component;
        gfn::serializable::Bool isComponentRoot;

        VertexProps();

        ///@brief serializes vertex props data into binary form, remember to free the buffer after read
        void serialize(binn* object);

        ///@brief deserializes binn binary data into vertex props
        void deserialize(void* object);

        ///@brief serializes vertex props data into json form (for clipboard)
        void serializeJson(nlohmann::json& j);

        ///@brief deserializes json data into vertex props (for clipboard)
        void deserializeJson(nlohmann::json& j);

        void get(const std::string& key, gfn::Args& output);

        void set(const std::string& key, const std::string& value, gfn::Args& output);
    };
}

#endif
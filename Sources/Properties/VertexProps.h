#ifndef VERTEXPROPS_H_
#define VERTEXPROPS_H_

#include <Objects/Serializable.h>
#include <binn.h>

namespace gfn {
    ///@brief Vertex properties that will be copied to the rendering thread / properties for the end user
    struct VertexProps {
        gfn::serializable::Uuid uuid;
        gfn::serializable::Bool enabled;
        gfn::serializable::Vec2f position;
        gfn::serializable::Color vertexFillColor;
        gfn::serializable::Double radius;
        gfn::serializable::Vec2f force;
        gfn::serializable::Bool pauseUpdate;
        gfn::serializable::Uuid component;
        gfn::serializable::Bool isComponentRoot;

        VertexProps();

        ///@brief serializes vertex props data into binary form, remember to free the buffer after read
        void serialize(binn* object);

        ///@brief deserializes binn binary data into vertex props
        void deserialize(void* object);

        void get(const std::string& key, gfn::Command& output);

        void set(const std::string& key, const std::string& value, gfn::Command& output);
    };
}

#endif
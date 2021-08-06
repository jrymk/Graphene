#ifndef EDGEPROPS_H
#define EDGEPROPS_H

#include <Core/Objects/Serializable.h>
#include <binn.h>

namespace gfn {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::serializable::Uuid edgeUuid;
        gfn::serializable::Bool enabled;
        gfn::serializable::Vec2f position;
        gfn::serializable::Uuid startVertexUuid;
        gfn::serializable::Vec2f startVertexPosition;
        gfn::serializable::Uuid endVertexUuid;
        gfn::serializable::Vec2f endVertexPosition;
        gfn::serializable::Color edgeFillColor;
        gfn::serializable::Double thickness;

        EdgeProps();

        ///@brief serializes edge props data into binary form, remember to free the buffer after read
        void serialize(binn* object);

        ///@brief deserializes binn binary data into edge props
        void deserialize(void* object);

        void get(const std::string& key, gfn::Args& output);

        void set(const std::string& key, const std::string& value, gfn::Args& output);
    };
}

#endif
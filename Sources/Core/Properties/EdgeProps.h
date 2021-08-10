#ifndef EDGEPROPS_H
#define EDGEPROPS_H

#include <Core/Objects/Serializable.h>
#include <binn.h>

namespace gfn {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::serializable::Uuid edgeUuid;
        gfn::serializable::String label;
        gfn::serializable::Double labelSize;
        gfn::serializable::Color labelColor;
        gfn::serializable::Bool enabled;
        gfn::serializable::Vec2f position;
        gfn::serializable::Uuid startVertexUuid;
        gfn::serializable::Vec2f startVertexPosition;
        gfn::serializable::Uuid endVertexUuid;
        gfn::serializable::Vec2f endVertexPosition;
        gfn::serializable::Color edgeColor;
        gfn::serializable::Double thickness;
        gfn::serializable::Vec2f force;
        gfn::serializable::Bool pauseUpdate;

        EdgeProps();

        ///@brief serializes edge props data into binary form, remember to free the buffer after read
        void serialize(binn* object);

        ///@brief deserializes binn binary data into edge props
        void deserialize(void* object);

        ///@brief serializes edge props data into json form (for clipboard)
        void serializeJson(nlohmann::json& j);

        ///@brief deserializes json data into edge props (for clipboard)
        void deserializeJson(nlohmann::json& j);

        void get(const std::string& key, gfn::Args& output);

        void set(const std::string& key, const std::string& value, gfn::Args& output);
    };
}

#endif
#pragma once

#include <Objects/Uuid.hpp>
#include <Properties/VertexProps.hpp>

namespace gfn::structure {
    class Component;

///@brief the data structure used internally for faster and more versatile access
    class Vertex {
    public:
        Component* component;

        gfn::properties::VertexProps* props;
        gfn::properties::VertexPropsInternal* internalProps;
    };
} // namespace gfn::core::structure
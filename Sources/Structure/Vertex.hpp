#pragma once

#include <Objects/Uuid.h>
#include <Properties/VertexProps.h>

namespace gfn {
    class Component;

///@brief the data structure used internally for faster and more versatile access
    class Vertex {
    public:
        Component* component;

        gfn::VertexProps* props;
    };
} // namespace gfn::core::structure
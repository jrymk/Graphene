#pragma once

#include <Core/Objects/Uuid.h>
#include <Core/Properties/EdgeProps.h>
#include <Core/Structure/Vertex.hpp>

namespace gfn {
///@brief the data structure used internally for faster and more versatile access
    class Edge {
    public:
        gfn::EdgeProps* props;

        gfn::Vertex* startVertex;
        gfn::Vertex* endVertex;
    };
} // namespace gfn::core::structure
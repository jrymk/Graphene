#pragma once

#include <Objects/Uuid.h>
#include <Properties/EdgeProps.h>
#include <Structure/Vertex.hpp>

namespace gfn {
///@brief the data structure used internally for faster and more versatile access
    class Edge {
    public:
        gfn::EdgeProps* props;

        gfn::Vertex* startVertex;
        gfn::Vertex* endVertex;
    };
} // namespace gfn::core::structure
#pragma once

#include <Objects/Uuid.hpp>
#include <Properties/EdgeProps.hpp>
#include <Structure/Vertex.hpp>

namespace gfn::structure {
///@brief the data structure used internally for faster and more versatile access
    class Edge {
    public:
        gfn::props::EdgeProps* props;

        gfn::structure::Vertex* startVertex;
        gfn::structure::Vertex* endVertex;
    };
} // namespace gfn::core::structure
#pragma once

#include <unordered_map>
#include <unordered_set>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Component/ComponentList.hpp>
#include <Structure/Vertex.hpp>
#include <Structure/Edge.hpp>
#include <Properties/Properties.hpp>
#include <Logging/Logging.hpp>

namespace gfn::structure {
/**
 * This is the class what the Graphene core actually touches.
 * Everything is stored in Vertex and Edge pointers rather than Uuids.
 * This is for faster, more versatile, and lock-free access to vertices.
 */
    class Structure {
    public:
        ComponentList componentList;
    };

} // namespace gfn::core::structure
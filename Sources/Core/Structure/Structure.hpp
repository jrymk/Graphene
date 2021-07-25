#pragma once

#include <unordered_map>
#include <unordered_set>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Component/ComponentList.hpp>
#include <Core/Structure/Vertex.hpp>
#include <Core/Structure/Edge.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Logging/Logging.hpp>

namespace gfn::core::structure {
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
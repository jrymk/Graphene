#pragma once

#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/EdgeProp.hpp>

namespace gfn::core::structure {
///@brief the data structure used internally for faster and more versatile access
class Edge {
  public:
	gfn::core::Uuid edgeUuid;
	gfn::core::Uuid startVertexUuid;
	gfn::core::Uuid endVertexUuid;

	gfn::core::properties::EdgeProp* prop;
};
} // namespace gfn::core::structure
#pragma once

#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/VertexProp.hpp>

class gfn::core::structure::Component;

namespace gfn::core::structure {
///@brief the data structure used internally for faster and more versatile access
class Vertex {
  public:
	gfn::core::Uuid uuid;

	Component* component;

	gfn::core::properties::VertexProp* prop;
};
} // namespace gfn::core::structure
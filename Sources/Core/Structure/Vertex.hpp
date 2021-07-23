#pragma once

#include <Core/Objects/Uuid.hpp>
#include <Core/Properties/VertexProp.hpp>

namespace gfn::core::structure {
class Component;

///@brief the data structure used internally for faster and more versatile access
class Vertex {
  public:
	gfn::core::Uuid uuid;

	Component* component;

	gfn::core::properties::VertexProp* prop;
};
} // namespace gfn::core::structure
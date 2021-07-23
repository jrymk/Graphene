#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class VertexProp {
  public:
	gfn::core::Uuid uuid;
	bool empty = true; // when true, this object acts like a null object, with basically no use
	gfn::core::Vec2f position;
	gfn::core::Uuid component;

	VertexProp(gfn::core::Uuid _uuid) : uuid(_uuid) { empty = false; }

	VertexProp() = default;
};
} // namespace gfn::core::data
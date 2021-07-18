#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class VertexProp {
  public:
	gfn::core::Uuid uuid;
	gfn::core::Vec2f position;
	gfn::core::Uuid component;

	VertexProp(gfn::core::Uuid _uuid) : uuid(_uuid) {}
};
} // namespace gfn::core::data
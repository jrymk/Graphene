#pragma once

#include <Core/Data/Properties/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class VertexProp {
  public:
	gfn::core::Uuid uuid;
	gfn::core::Vec2f position;

	VertexProp() = default;
};
} // namespace gfn::core::data
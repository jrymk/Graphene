#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class EdgeProp {
  public:
	gfn::core::Uuid uuid;

	EdgeProp(gfn::core::Uuid _uuid) : uuid(_uuid) {}
};
} // namespace gfn::core::data
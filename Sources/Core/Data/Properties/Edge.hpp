#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class EdgeProp {
  public:
	gfn::core::Uuid uuid;
	bool empty = true; // when true, this object acts like a null object, with basically no use

	EdgeProp(gfn::core::Uuid _uuid) : uuid(_uuid) { empty = false; }

	EdgeProp() = default;
};
} // namespace gfn::core::data
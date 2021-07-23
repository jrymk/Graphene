#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class ComponentProp {
  public:
	gfn::core::Uuid uuid;
	bool empty = true; // when true, this object acts like a null object, with basically no use

	ComponentProp(gfn::core::Uuid _uuid) : uuid(_uuid) { empty = false; }

	ComponentProp() = default;
};
} // namespace gfn::core::data
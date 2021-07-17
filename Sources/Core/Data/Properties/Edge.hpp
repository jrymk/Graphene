#pragma once

#include <Core/Data/Properties/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class EdgeProp {
  public:
	gfn::core::Uuid uuid;

	EdgeProp() = default;
};
} // namespace gfn::core::data
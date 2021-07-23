#pragma once

#include <Core/Data/Uuid.hpp>
#include <Core/Objects/Vec2f.hpp>

namespace gfn::core::data {
class BiconnectedComponentProp {
  public:
	enum Type {
		BLOCK,
		ARTICULATION,
	};

	gfn::core::Uuid uuid;
	bool empty = true; // when true, this object acts like a null object, with basically no use
	gfn::core::data::BiconnectedComponentProp::Type type;

	BiconnectedComponentProp(gfn::core::Uuid _uuid, gfn::core::data::BiconnectedComponentProp::Type _type)
		: uuid(_uuid), type(_type) {
		empty = false;
	}

	BiconnectedComponentProp() = default;
};
} // namespace gfn::core::data
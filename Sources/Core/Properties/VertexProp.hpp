#pragma once

#include <Core/Objects/Uuid.hpp>

namespace gfn::core::properties {
struct VertexProp {
	gfn::core::Uuid uuid;
	bool present = false;

	gfn::core::Uuid _prevComponent = gfn::core::uuid::createNil();
	bool _prevComponentRoot = false;
};
} // namespace gfn::core::properties
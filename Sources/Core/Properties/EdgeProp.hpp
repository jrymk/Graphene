#pragma once

#include <Core/Objects/Uuid.hpp>

namespace gfn::core::properties {
struct EdgeProp {
	gfn::core::Uuid edgeUuid;
	bool present = false;

	gfn::core::Uuid startVertexUuid;
	gfn::core::Uuid endVertexUuid;
};
} // namespace gfn::core::properties
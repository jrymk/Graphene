#pragma once

#include <Core/Objects/Vec2f.hpp>
#include <Core/Objects/Uuid.hpp>
#include <System/Random/Random.hpp>

namespace gfn::core::properties {
struct VertexProp {
	gfn::core::Uuid uuid;
	bool present;
    gfn::core::Vec2f position;
    gfn::core::Vec2f force;

	gfn::core::Uuid _component = gfn::core::uuid::createNil();
	bool _isComponentRoot = false;
	gfn::core::Vec2f _componentCentroidPosition;

	VertexProp() {
		uuid = gfn::core::uuid::createNil();
		present = false;
		std::uniform_real_distribution dis(-0.5, 0.5);
		position = gfn::core::Vec2f(dis(gfn::system::random::getEngine()), dis(gfn::system::random::getEngine()));
	}
};
} // namespace gfn::core::properties
#pragma once

#include <Objects/Vec2f.hpp>
#include <Objects/Uuid.hpp>
#include <System/Random/Random.hpp>

namespace gfn::properties {
    ///@brief Vertex properties that will be copied to the rendering thread / properties for the end user
    struct VertexProps {
        gfn::Uuid uuid;
        bool enabled;
        gfn::Vec2f position;

        VertexProps() {
            uuid = gfn::uuid::createNil();
            enabled = false;
            position = Vec2f();
        }
    };

    ///@brief Vertex properties used in core / internal properties for graph drawing and such
    struct VertexPropsInternal {
        gfn::Vec2f force;

        gfn::Vec2f componentCentroidPosition;
        gfn::Uuid component = gfn::uuid::createNil();
        bool isComponentRoot = false;
    };
} // namespace gfn::core::properties
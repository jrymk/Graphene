#pragma once

#include <Objects/Vec2f.hpp>
#include <Objects/Uuid.hpp>
#include <Objects/Color.hpp>

namespace gfn::properties {
    ///@brief Vertex properties that will be copied to the rendering thread / properties for the end user
    struct VertexProps {
        gfn::Uuid uuid;
        bool enabled;
        gfn::Vec2f position;

        gfn::Color vertexFillColor;
        double radius;
        double radiusOuter;

        VertexProps() {
            uuid = gfn::uuid::createNil();
            enabled = false;
            position = Vec2f();
            vertexFillColor.setRGBA(255, 211, 0, 255);
            radius = 0.5;
            radiusOuter = 0.8;
        }
    };

    ///@brief Vertex properties used in core / internal properties for graph drawing and such
    struct VertexPropsInternal {
        gfn::Vec2f force = gfn::Vec2f(0.0, 0.0);

        gfn::Vec2f componentCentroidPosition;
        gfn::Uuid component = gfn::uuid::createNil();
        bool isComponentRoot = false;
    };
} // namespace gfn::core::properties
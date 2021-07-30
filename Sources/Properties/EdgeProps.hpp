#pragma once

#include <Objects/Uuid.hpp>
#include <Objects/Color.hpp>

namespace gfn::properties {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::Uuid edgeUuid;
        bool enabled;
        gfn::Vec2f position;

        gfn::Uuid startVertexUuid;
        gfn::Vec2f startVertexPosition;
        gfn::Uuid endVertexUuid;
        gfn::Vec2f endVertexPosition;

        gfn::Color edgeFillColor;
        double thickness;
        gfn::Color edgeNodeColor;
        double radius;

        EdgeProps() {
            enabled = false;
            edgeFillColor.setRGBA(0, 0, 0, 255);
            thickness = 0.3;
            edgeNodeColor.setRGBA(255, 0, 0, 50);
            radius = 0.2;
        }
    };

    ///@brief Edge properties used in core / internal properties for graph drawing and such
    struct EdgePropsInternal {
        gfn::Vec2f force = gfn::Vec2f(0.0, 0.0);
    };

} // namespace gfn::core::properties
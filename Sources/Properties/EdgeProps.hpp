#pragma once

#include <Objects/Uuid.hpp>
#include <Objects/Color.hpp>

namespace gfn::properties {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::Uuid edgeUuid;
        bool enabled;

        gfn::Uuid startVertexUuid;
        gfn::Uuid endVertexUuid;

        gfn::Color edgeFillColor;
        double thickness;

        EdgeProps() {
            enabled = false;
            edgeFillColor.setRGBA(0, 0, 0, 255);
            thickness = 0.2;
        }
    };

    ///@brief Edge properties used in core / internal properties for graph drawing and such
    struct EdgePropsInternal {

    };

} // namespace gfn::core::properties
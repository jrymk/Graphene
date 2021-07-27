#pragma once

#include <Objects/Uuid.hpp>

namespace gfn::properties {
    ///@brief Edge properties that will be copied to the rendering thread / properties for the end user
    struct EdgeProps {
        gfn::Uuid edgeUuid;
        bool enabled;

        gfn::Uuid startVertexUuid;
        gfn::Uuid endVertexUuid;

        EdgeProps() {
            enabled = false;
        }
    };

    ///@brief Edge properties used in core / internal properties for graph drawing and such
    struct EdgePropsInternal {

    };

} // namespace gfn::core::properties
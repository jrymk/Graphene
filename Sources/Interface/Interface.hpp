#pragma once

#include <Interface/TripleBuffer.hpp>
#include <Graph/Graph.h>
#include <Properties/Properties.h>
#include <Configs/Configs.hpp>
#include <Core/CmdBuffer.hpp>
#include <Core/CmdBuffer.hpp>
#include <json.hpp>
#include <binn.h>

#undef interface // I just really want to use this word

namespace gfn::structure {
    class Structure;
}

namespace gfn {
    class Interface {
    public:
        // core output stream
        TripleBuffer<gfn::Graph> graph;
        TripleBuffer<gfn::Properties> props;
        TripleBuffer<gfn::configs::Configs> cfg;

        TripleBuffer<gfn::cmdbuffer::CmdBuffer> cmdBuf;

        Interface() = default;
    };
} // namespace gfn::interface
#pragma once

#include <Interface/TripleBuffer.hpp>
#include <UserGraph/UserGraph.hpp>
#include <Properties/Properties.hpp>
#include <Configs/Configs.hpp>
#include <Core/CmdBuffer.hpp>
#include <json.hpp>
#include <binn.h>

#undef interface // I just really want to use this word

namespace gfn::interface {
    class Interface {
    public:
        // this standalone data structure of user props will safely export graph data from core
        TripleBuffer<gfn::props::Properties> properties;
        // this gives a way for the main thread to obtain the graph structure
        TripleBuffer<gfn::usergraph::UserGraph> usergraph;
        // this non working log buffer will soon push logs to the terminal, gui or a log file
        TripleBuffer<gfn::logging::LogBuffer> logBuffer;
        // this stores the configurations for core to run, from the user to the core
        TripleBuffer<gfn::configs::Configs> configs;

        TripleBuffer<gfn::cmdbuffer::CmdBuffer> cmdBuffer;
    };
} // namespace gfn::interface
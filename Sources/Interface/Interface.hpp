#pragma once

#include <Interface/TripleBuffer.hpp>
#include <Properties/UserProps.hpp>
#include <Configs/Configs.hpp>
#include <Core/Command/CmdBuffer.hpp>

#undef interface // I just really want to use this word

namespace gfn::interface {
    class Interface {
    public:
        // this standalone data structure of user props will safely export graph data from core
        TripleBuffer<gfn::properties::UserProps> userprops;
        // this non working log buffer will soon push logs to the terminal, gui or a log file
        TripleBuffer<gfn::logging::LogBuffer> logBuffer;
        // this stores the configurations for core to run, from the user to the core
        TripleBuffer<gfn::configs::Configs> configs;

        TripleBuffer<gfn::cmdbuffer::CmdBuffer> cmdBuffer;
    };
} // namespace gfn::interface
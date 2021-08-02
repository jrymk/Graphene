#pragma once

#include <Objects/Command.hpp>
#include <deque>

namespace gfn::cmdbuffer {
    class CmdBuffer {
    public:
        std::deque<gfn::Command> commands;
    };
}
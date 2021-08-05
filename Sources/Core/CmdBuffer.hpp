#pragma once

#include <Objects/Command.h>
#include <deque>

namespace gfn::cmdbuffer {
    class CmdBuffer {
    public:
        std::deque<gfn::Command> commands;
    };
}
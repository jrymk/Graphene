#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include <deque>
#include <Core/Objects/Args.h>

namespace gfn {
    class ArgsStream {
    public:
        std::deque<gfn::Args> buffer;
    };
}

#endif
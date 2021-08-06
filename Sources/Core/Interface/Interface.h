#ifndef INTERFACE_H
#define INTERFACE_H

#include <Core/Interface/TripleBuffer.h>
#include <Core/Graph/Graph.h>
#include <Core/Properties/Properties.h>
#include <Core/Configs/Configs.hpp>
#include <Core/Core/ArgsStream.h>

#undef interface

namespace gfn {
    class Interface {
    public:
        TripleBuffer<gfn::Graph> graph;          // core ->  // Contains the structure, properties and configs
        TripleBuffer<gfn::ArgsStream> commands;  // core <-
        TripleBuffer<gfn::ArgsStream> responses; // core ->

        Interface();
    };
}

#endif
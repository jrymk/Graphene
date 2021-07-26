#pragma once

#include <Core/Logging/Logging.hpp>
#include <Core/UserGraph/UserGraph.hpp>
#include <Core/Structure/Structure.hpp>
#include <Core/Configs/Configs.hpp>
#include <Core/Properties/Properties.hpp>
#include <Core/Parser/Parser.hpp>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/Drawlgo.hpp>
#include <atomic>

namespace gfn::core {
    class Core {
    public:
        gfn::interface::Interface *interface;
        gfn::core::properties::Properties properties;
        gfn::core::usergraph::UserGraph usergraph;
        gfn::core::structure::Structure structure;
        gfn::core::drawlgo::Drawlgo drawlgo;
        gfn::core::parser::Parser parser;

        Core(gfn::interface::Interface *interface) : interface(interface) {
            usergraph.bindProperties(&properties);
            structure.componentList.bindProperties(&properties);
            parser.usergraph = &usergraph;
        }

        void coreCycle() {
            gfn::core::logging::logBuffer = &interface->logBuffer.getWriteBuffer();
            static bool first = true;
            if (first) {
                auto a = parser.execute("graph new vertex");
                auto b = parser.execute("graph new vertex");
                auto c = parser.execute("graph new vertex");
                auto d = parser.execute("graph new vertex");
                auto e = parser.execute("graph new vertex");
                parser.execute("graph new edge " + a + " " + b);
                parser.execute("graph new edge " + c + " " + b);
                parser.execute("graph new edge " + e + " " + d);
                parser.execute("graph new edge " + b + " " + d);
                first = false;
            }

            drawlgo.update(interface->configs.getReadBuffer(), &structure, &properties);

            if (usergraph.pendingUpdate) {
                // update component list with usergraph and rebuild block cut tree (currently all)
                structure.componentList.componentify(&usergraph);
                usergraph.pendingUpdate = false;
            }

            // std::cerr << usergraph.getAdjList().size() << "\n";
            /// TODO: graph update stuff

            if (interface->properties.pendingWrite()) {
                // assignment operator, writes the core content to the write buffer
                interface->properties.getWriteBuffer() = properties;
                interface->properties.writeDone();
            }

            if (interface->logBuffer.pendingWrite()) {
                // assignment operator, writes the core content to the write buffer
                // interface->logBuffer.writeDone();
            }

            interface->configs.readDone();
        }
    };
} // namespace gfn::core
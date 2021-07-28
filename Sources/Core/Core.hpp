#pragma once

#include <Logging/Logging.hpp>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Structure.hpp>
#include <Configs/Configs.hpp>
#include <Properties/Properties.hpp>
#include <Core/Parser/Parser.hpp>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/Drawlgo.hpp>
#include <atomic>

namespace gfn::core {
    class Core {
    public:
        gfn::interface::Interface* interface;
        gfn::properties::Properties properties;
        gfn::usergraph::UserGraph usergraph;
        gfn::structure::Structure structure;
        gfn::core::drawlgo::Drawlgo drawlgo;
        gfn::core::parser::Parser parser;

        Core() = default;

        void bindInterface(gfn::interface::Interface* _interface) {
            this->interface = _interface;
            usergraph.bindProperties(&properties);
            structure.componentList.bindSource(&usergraph, &properties);
            parser.usergraph = &usergraph;
        }

        void coreCycle() {
            // gfn::core::logging::logBuffer = &interface->logBuffer.getWrite();
            static bool first = true;
            if (first) {
                for (int k = 0; k < 1; k++) {
                    gfn::Uuid a, b;
                    a = parser.execute("graph new vertex");
                    for (int i = 0; i < 30; i++) {
                        b = parser.execute("graph new vertex");
                        parser.execute("graph new edge " + a + " " + b);
                        a = b;
                    }
                }
                first = false;
            }

            // gfn::timer::Timer updateTimer;
            drawlgo.update(interface->configs.getRead(), &structure, &properties);
            // std::cerr << "Graph update took " << updateTimer.getMicroseconds() << "us\n";

            if (usergraph.pendingUpdate) {
                // update component list with usergraph and rebuild block cut tree (currently all)
                structure.componentList.componentify();
                usergraph.pendingUpdate = false;
            }

            // std::cerr << usergraph.getAdjList().size() << "\n";
            /// TODO: graph update stuff

            if (interface->userprops.wantWrite()) {
                // assignment operator, writes the core content to the write buffer
                properties.exportToUserProps(interface->userprops.getWrite());
                interface->userprops.writeDone();
            }

            if (interface->logBuffer.wantWrite()) {
                // assignment operator, writes the core content to the write buffer
                // interface->logBuffer.writeDone();
            }

            interface->configs.readDone();
            // std::cerr << "Write took " << writeTimer.getMicroseconds() << "us\n";
        }
    };
} // namespace gfn::core
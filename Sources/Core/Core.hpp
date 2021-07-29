#pragma once

#include <Logging/Logging.hpp>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Structure.hpp>
#include <Configs/Configs.hpp>
#include <Properties/Properties.hpp>
#include <Properties/PropertiesCmd.hpp>
#include <Core/Command/CmdInterface.hpp>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/Drawlgo.hpp>
#include <Core/Command/CmdBuffer.hpp>
#include <atomic>

namespace gfn::core {
    class Core {
    public:
        gfn::interface::Interface* interface;
        gfn::properties::Properties properties;
        gfn::usergraph::UserGraph usergraph;
        gfn::structure::Structure structure;
        gfn::core::drawlgo::Drawlgo drawlgo;
        gfn::core::cmd::CmdInterface cmdInterface;

        Core() = default;

        void bindInterface(gfn::interface::Interface* _interface) {
            this->interface = _interface;
            usergraph.bindProperties(&properties);
            structure.componentList.bindSource(&usergraph, &properties);
            cmdInterface.usergraph = &usergraph;
        }

        void coreCycle() {
            // gfn::core::logging::logBuffer = &interface->logBuffer.getWrite();
            while (!interface->cmdBuffer.getRead()->commands.empty()) {
                gfn::Command output;

                usergraph.tryInterpret(interface->cmdBuffer.getRead()->commands.front(), output);
                gfn::properties::tryInterpret(&properties, interface->cmdBuffer.getRead()->commands.front(), output);

                std::cout << output.getString() << "\n";
                interface->cmdBuffer.getRead()->commands.pop_front();
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
            interface->cmdBuffer.readDone();
            // std::cerr << "Write took " << writeTimer.getMicroseconds() << "us\n";
        }
    };
} // namespace gfn::core
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
        gfn::configs::Configs configs;
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

                std::cout << "Command: " << interface->cmdBuffer.getRead()->commands.front().getString();
                usergraph.tryParse(interface->cmdBuffer.getRead()->commands.front(), output);
                gfn::properties::tryParse(&properties, interface->cmdBuffer.getRead()->commands.front(), output);
                configs.tryParse(interface->cmdBuffer.getRead()->commands.front(), output);

                std::cout << output.getString() << "\n";
                interface->cmdBuffer.getRead()->commands.pop_front();
            }

            // gfn::timer::Timer updateTimer;
            drawlgo.update(&configs, &structure, &properties);
            // std::cerr << "Graph update took " << updateTimer.getMicroseconds() << "us\n";

            if (usergraph.pendingUpdate) {
                // update component list with usergraph and rebuild block cut tree (currently all)
                structure.componentList.componentify();
                usergraph.pendingUpdate = false;
                interface->usergraph.getWriteRef() = usergraph;
            }

            // std::cerr << usergraph.getAdjList().size() << "\n";
            /// TODO: graph update stuff

            if (interface->userprops.wantWrite()) {
                // assignment operator, writes the core content to the write buffer
                // not the fastest way to do it, but whatever...
                properties.exportToUserProps(interface->userprops.getWrite());
                interface->userprops.writeDone();
            }

            if (interface->logBuffer.wantWrite()) {
                // assignment operator, writes the core content to the write buffer
                // interface->logBuffer.writeDone();
            }
            interface->configs.getWriteRef() = configs;
            interface->configs.writeDone();
            interface->cmdBuffer.readDone();
            // std::cerr << "Write took " << writeTimer.getMicroseconds() << "us\n";
        }
    };
} // namespace gfn::core
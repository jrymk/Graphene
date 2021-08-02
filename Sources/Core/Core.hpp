#pragma once

#include <Logging/Logging.hpp>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Structure.hpp>
#include <Configs/Configs.hpp>
#include <Properties/Properties.hpp>
#include <Properties/PropertiesCmd.hpp>
#include <Interface/Interface.hpp>
#include <Core/Drawlgo/Drawlgo.hpp>
#include <Core/CmdBuffer.hpp>
#include <atomic>
#include <iomanip>
#include <Core/Parser.hpp>

namespace gfn::core {
    class Core {
    public:
        gfn::interface::Interface* interface;
        gfn::props::Properties properties;
        gfn::usergraph::UserGraph usergraph;
        gfn::structure::Structure structure;
        gfn::configs::Configs configs;
        gfn::core::drawlgo::Drawlgo drawlgo;
        gfn::parser::Parser parser;

        explicit Core(gfn::interface::Interface* _interface) :
                interface(_interface),
                properties(),
                usergraph(),
                structure(&usergraph, &properties),
                parser(interface, &properties, &usergraph, &structure, &configs) {
            usergraph.bindProperties(&properties);
        }

        void coreCycle() {
            parser.parseAll();

            if (usergraph.pendingUpdate) {
                // update component list with usergraph and rebuild block cut tree
                structure.componentList.componentify();
                usergraph.pendingUpdate = false;
                interface->usergraph.getWrite() = usergraph;
                /// TODO: partial update
            }

            drawlgo.update(&configs, &structure, &properties);

            if (interface->properties.wantWrite()) {
                interface->properties.getWrite() = properties;
                interface->properties.writeDone();
            }

            interface->configs.getWrite() = configs;
            interface->configs.writeDone();
        }
    };
} // namespace gfn::core
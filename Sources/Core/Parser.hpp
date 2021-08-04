#pragma once

#include <Interface/Interface.hpp>
#include <Properties/Properties.hpp>
#include <Properties/PropertiesCmd.hpp>
#include <UserGraph/UserGraph.hpp>
#include <Structure/Structure.hpp>
#include <Configs/Configs.hpp>
#include <Core/File.hpp>

namespace gfn::parser {
    class Parser {
        gfn::interface::Interface* interface;
        gfn::props::Properties* properties;
        gfn::usergraph::UserGraph* usergraph;
        gfn::structure::Structure* structure;
        gfn::configs::Configs* configs;

    public:
        Parser(gfn::interface::Interface* _interface, gfn::props::Properties* _props,
               gfn::usergraph::UserGraph* _usergraph, gfn::structure::Structure* _structure,
               gfn::configs::Configs* _configs) :
                interface(_interface),
                properties(_props),
                usergraph(_usergraph),
                structure(_structure),
                configs(_configs) {
        }

        void parse(Command command, gfn::Command& output) {
            std::string cmd = command.getParamValue("command");
            if (cmd.empty()) return;
            else if (cmd == "open") file::open(command, output, properties, usergraph, configs);
            else if (cmd == "save") file::save(command, output, properties, usergraph, configs);
            else if (cmd == "configs") configs->parse(command, output);
            else if (cmd == "mkvertex") usergraph->addVertex(command, output);
            else if (cmd == "rmvertex") usergraph->removeVertex(command, output);
            else if (cmd == "mkedge") usergraph->addEdge(command, output);
            else if (cmd == "rmedge") usergraph->removeEdge(command, output);
            else if (cmd == "setvertexprops") gfn::props::setVertexProp(properties, command, output);
            else if (cmd == "getvertexprops") gfn::props::getVertexProp(properties, command, output);
            else if (cmd == "setedgeprops") gfn::props::setEdgeProp(properties, command, output);
            else if (cmd == "getedgeprops") gfn::props::getEdgeProp(properties, command, output);
            else if (cmd == "help") {
                output.newParam("-output", "Available commands: "
                                           "select\topen\tsave\tconfigs\tmkvertex\trmvertex\tmkedge\trmedge\tsetvertexprops\t"
                                           "getvertexprops\tsetedgeprops\tgetedgeprops\thelp");
            }
        }

        void parseAll() {
            while (!interface->cmdBuffer.getRead()->commands.empty()) {
                gfn::Command output;
                //std::cout << "received: " << interface->cmdBuffer.getRead()->commands.front().getString() << "\n";
                parse(interface->cmdBuffer.getRead()->commands.front(), output);
                //std::cout << output.getString() << "\n";

                if (output.getParamValue("successful") == "false" || output.getFlag("-error")
                    || output.getFlag("-warning")) {
                    std::cout << interface->cmdBuffer.getRead()->commands.front().getString() << "\n";
                    std::cerr << output.getString() << "\n";
                }
                /*else
                    std::cout << output.getString() << "\n";*/

                interface->cmdBuffer.getRead()->commands.pop_front();
            }
            interface->cmdBuffer.readDone();
        }

    };
}
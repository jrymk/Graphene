#pragma once

#include <Interface/Interface.hpp>
#include <Properties/Properties.h>
#include <Graph/Graph.h>
#include <Structure/Structure.h>
#include <Configs/Configs.hpp>
#include <Core/File.hpp>
#include <minitrace.h>

namespace gfn::parser {
    class Parser {
        gfn::Interface* itf;

    public:
        explicit Parser(gfn::Interface* _itf) : itf(_itf) {}

        void parse(Command command, gfn::Command& output) {
            std::string cmd = command.getParamValue("command");
            if (cmd.empty()) return;
            else if (cmd == "open") file::open(command, output, itf);
            else if (cmd == "save") file::save(command, output, itf);
            else if (cmd == "configs") itf->cfg.getWrite().parse(command, output);
            else if (cmd == "mkvertex") itf->graph.getWrite().addVertex(command, output);
            else if (cmd == "rmvertex") itf->graph.getWrite().removeVertex(command, output);
            else if (cmd == "mkedge") itf->graph.getWrite().addEdge(command, output);
            else if (cmd == "rmedge") itf->graph.getWrite().removeEdge(command, output);
            else if (cmd == "setvertexprops") itf->props.getWrite().setVertexProp(command, output);
            else if (cmd == "getvertexprops") itf->props.getWrite().getVertexProp(command, output);
            else if (cmd == "setedgeprops") itf->props.getWrite().setEdgeProp(command, output);
            else if (cmd == "getedgeprops") itf->props.getWrite().getEdgeProp(command, output);
            else if (cmd == "help") {
                output.newParam("-output", "Available commands: "
                                           "select\topen\tsave\tconfigs\tmkvertex\trmvertex\tmkedge\trmedge\tsetvertexprops\t"
                                           "getvertexprops\tsetedgeprops\tgetedgeprops\thelp");
            }
        }

        void parseAll() {
            MTR_SCOPE("core", "parse commands");

            while (!itf->cmdBuf.getRead()->commands.empty()) {
                gfn::Command output;
                //std::cout << "received: " << interface->cmdBuffer.getRead()->commands.front().getString() << "\n";
                parse(itf->cmdBuf.getRead()->commands.front(), output);
                //std::cout << output.getString() << "\n";

                if (output.getParamValue("successful") == "false" || output.getFlag("-error")
                    || output.getFlag("-warning")) {
                    std::cout << itf->cmdBuf.getRead()->commands.front().getString() << "\n";
                    std::cerr << output.getString() << "\n";
                }
                /*else
                    std::cout << output.getString() << "\n";*/

                itf->cmdBuf.getRead()->commands.pop_front();
            }
            itf->cmdBuf.readDone();
        }

    };
}
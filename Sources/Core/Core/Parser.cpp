#include "Core.h"
#include <Tracy.hpp>

namespace gfn {
    void Core::parse(Args command, gfn::Args& output) {
        ZoneScoped

        std::string cmd = command.getParamValue("command");
        if (cmd.empty()) return;
        else if (cmd == "open") openFile(command, output);
        else if (cmd == "save") saveFile(command, output);
        else if (cmd == "configs") itf.graph.getWrite().cfg.parse(command, output);
        else if (cmd == "mkvertex") itf.graph.getWrite().addVertex(command, output);
        else if (cmd == "rmvertex") itf.graph.getWrite().removeVertex(command, output);
        else if (cmd == "mkedge") itf.graph.getWrite().addEdge(command, output);
        else if (cmd == "rmedge") itf.graph.getWrite().removeEdge(command, output);
        else if (cmd == "setvertexprops") itf.graph.getWrite().props.setVertexProp(command, output);
        else if (cmd == "getvertexprops") itf.graph.getWrite().props.getVertexProp(command, output);
        else if (cmd == "setedgeprops") itf.graph.getWrite().props.setEdgeProp(command, output);
        else if (cmd == "getedgeprops") itf.graph.getWrite().props.getEdgeProp(command, output);
        else if (cmd == "help") {
            output.newParam("-output", "Available commands: "
                                       "select\topen\tsave\tconfigs\tmkvertex\trmvertex\tmkedge\trmedge\tsetvertexprops\t"
                                       "getvertexprops\tsetedgeprops\tgetedgeprops\thelp");
        }
    }

    void Core::parseCommands() {
        ZoneScoped

        while (!itf.commands.getRead()->buffer.empty()) {
            std::cout << "Parsing: " << itf.commands.getRead()->buffer.front().getString() << "\n";
            gfn::Args output;
            parse(itf.commands.getRead()->buffer.front(), output);

            if (output.getParamValue("successful") == "false" || output.getFlag("-error")
                || output.getFlag("-warning")) {
                std::cerr << output.getString() << "\n";
            } else
                std::cout << output.getString() << "\n";

            itf.commands.getRead()->buffer.pop_front();
        }
        itf.commands.readDone();
    }
}
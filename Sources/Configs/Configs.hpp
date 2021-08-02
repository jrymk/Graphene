#pragma once

#include <Objects/Parsables.hpp>

namespace gfn::configs {
    class Configs {
    public:
        gfn::parsables::Double c1;
        gfn::parsables::Double c2;
        gfn::parsables::Double c3;
        gfn::parsables::Double c4;
        gfn::parsables::Double c5;
        gfn::parsables::Double c6;

        Configs() :
                c1("c1", 2.0),
                c2("c2", 1.0),
                c3("c3", 1.0),
                c4("c4", 0.12),
                c5("c5", 0.1),
                c6("c6", 0.0001) {
        }

        bool parse(gfn::Command command, gfn::Command& output) {
            if (command.getParamValue("command") == "configs") {
                if (!command.getParamValue("-c1").empty())
                    c1.setValueStr(command.getParamValue("-c1"), output);
                if (!command.getParamValue("-c2").empty())
                    c2.setValueStr(command.getParamValue("-c2"), output);
                if (!command.getParamValue("-c3").empty())
                    c3.setValueStr(command.getParamValue("-c3"), output);
                if (!command.getParamValue("-c4").empty())
                    c4.setValueStr(command.getParamValue("-c4"), output);
                if (!command.getParamValue("-c5").empty())
                    c5.setValueStr(command.getParamValue("-c5"), output);
                if (!command.getParamValue("-c6").empty())
                    c6.setValueStr(command.getParamValue("-c6"), output);
                return false;
                return true;
            }
            return false;
        }

    };
}
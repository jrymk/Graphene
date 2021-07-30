#pragma once

#include <Parser/Parser.hpp>

namespace gfn::configs {
    class Configs {
    public:
        double c1 = 2.0;
        double c2 = 1.0;
        double c3 = 1.0;
        double c4 = 0.2;
        double c5 = 0.1;
        double c6 = 0.0001;

        bool tryParse(gfn::Command command, gfn::Command& output) {
            if (command.getParamValue("command") == "constant") {
                if (!command.getParamValue("-c1").empty())
                    gfn::parser::parseFloat(c1, command.getParamValue("-c1"), output);
                if (!command.getParamValue("-c2").empty())
                    gfn::parser::parseFloat(c2, command.getParamValue("-c2"), output);
                if (!command.getParamValue("-c3").empty())
                    gfn::parser::parseFloat(c3, command.getParamValue("-c3"), output);
                if (!command.getParamValue("-c4").empty())
                    gfn::parser::parseFloat(c4, command.getParamValue("-c4"), output);
                if (!command.getParamValue("-c5").empty())
                    gfn::parser::parseFloat(c5, command.getParamValue("-c5"), output);
                if (!command.getParamValue("-c6").empty())
                    gfn::parser::parseFloat(c6, command.getParamValue("-c6"), output);
                return true;
            }
            return false;
        }

    };
}
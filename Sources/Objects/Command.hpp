#pragma once

#include <string>
#include <sstream>

namespace gfn {
    typedef std::string Command;
}

namespace gfn::command {
    class Parser {
        std::stringstream ss;

    public:
        explicit Parser(const gfn::Command& cmd) {
            ss.str(cmd);
        }

        gfn::Command readWord() {
            gfn::Command word;
            ss >> word;
            return word;
        }

        gfn::Command getRemaining() {
            gfn::Command cmd;
            std::getline(ss, cmd);
            return cmd;
        }
    };
}


#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>
#include <sstream>

namespace gfn {
    class Args {
    public:
        std::vector<std::pair<std::string, std::string>> params;

        Args();

        /*
         * Args parser
         *
         *   Examples:
         *     setprop vertex -name="vertex 5" -key=position -value=(-2,3.5)
         *     would be parsed into
         *     {
         *        {"command", "setprop vertex"},
         *        {"-name"  , "vertex 5"      },
         *        {"-value" , "(-2,3.5)"      }
         *     }
         *
         *   Simply put, the "command" entry is whatever comes before any '-' character,
         *   Key or value that contain spaces need to be surrounded with double quotes
         *   Extra spaces are allowed, spaces around '=' is not.
         *
         *   extra note: To set a prop you can do key(value) or keys(val,ues), for example rgba(0,0,0,255) or a(255)
         *               rgba(0, 0, 0, 255) IS allowed, but remember to add quotes
         */
        explicit Args(const std::string& command);

        std::string getParamValue(const std::string& paramKey);

        bool getFlag(const std::string& paramKey);

        void newParam(const std::string& key, const std::string& value);

        void eraseParam(const std::string& key);

        std::string getString();
    };
}

#endif
#pragma once

#include <Objects/Command.hpp>
#include <string>
#include <sstream>

namespace gfn::parser {
    void parseBool(bool& dest, const std::string& value, gfn::Command& output) {
        if (value == "true") {
            output.newParam("-parse-successful", "true");
            output.newParam("-value", "true");
            dest = true;
            return;
        }
        if (value == "false") {
            output.newParam("-parse-successful", "true");
            output.newParam("-value", "false");
            dest = false;
            return;
        }
        output.newParam("-parse-successful", "false");
        output.newParam("-error", "PARSE_ERROR");
        output.newParam("-fix", "Expected true or false");
    }

    void parseInt(int& dest, const std::string& value, gfn::Command& output) {
        std::stringstream ss(value);
        int i;
        if (ss >> i) {
            output.newParam("-parse-successful", "true");
            output.newParam("-value", std::to_string(i));
            dest = i;
            return;
        }
        output.newParam("-parse-successful", "false");
        output.newParam("-error", "PARSE_ERROR");
        output.newParam("-fix", "Expected integer number");
    }

    void parseFloat(double& dest, const std::string& value, gfn::Command& output) {
        std::stringstream ss(value);
        double f;
        if (ss >> f) {
            output.newParam("-parse-successful", "true");
            output.newParam("-value", std::to_string(f));
            dest = f;
            return;
        }
        output.newParam("-parse-successful", "false");
        output.newParam("-error", "PARSE_ERROR");
        output.newParam("-fix", "Expected floating point number");
    }
}
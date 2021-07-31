#pragma once

#include <string>
#include <vector>
#include <queue>
#include <sstream>

namespace gfn {
    class Command {
    public:
        std::vector<std::pair<std::string, std::string>> params;

        Command() = default;

        Command(const std::string& command) {
            std::stringstream ss;
            ss.str(command);
            std::queue<std::string> q;
            std::string temp;
            while (ss >> temp)
                q.push(temp);
            if (q.empty())
                return;

            // read the command part
            // mkvertex -name="Police office"     setprop vertex -key=position -value=+(-5.0, 10.0)
            // ^^^^^^^^                           ^^^^^^^^^^^^^^
            std::string cmd;
            while (!q.empty() && q.front()[0] != '-') {
                if (!cmd.empty())
                    cmd += " ";
                cmd += q.front();
                q.pop();
            }
            params.emplace_back("command", cmd);

            // read the command part
            // mkvertex -name="Police office"     setprop vertex -key=position -value=+(-5.0, 10.0)
            //          ^^^^^^^^^^^^^^^^^^^^^                    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
            while (!q.empty()) {
                auto s = q.front();
                if (s[0] != '-') {
                    // parse error, reached non - starting param
                    std::cerr << command << "\n" << "PARSE_ERROR: did you forgot to add - before parameters\n";
                    q.pop();
                    continue;
                }
                auto equalPos = s.find('=');
                std::string key = s.substr(0, equalPos); // does not include the - in the beginning -> -name
                std::string value = s.substr(equalPos + 1, s.size() - equalPos - 1); // "Police office"
                if (*value.begin() == '"' && *value.rbegin() == '"') {
                    value.erase(value.begin());
                    value.erase(value.end() - 1);
                } // Police office

                params.emplace_back(key, value);
                q.pop();
            }
        }

        std::string getParamValue(const std::string& paramKey) {
            for (auto& e : params) {
                if (e.first == paramKey)
                    return e.second;
            }
            return "";
        }

        bool getFlag(const std::string& paramKey) {
            return !getParamValue(paramKey).empty();
        }

        void newParam(const std::string& key, const std::string& value) {
            params.emplace_back(key, value);
        }

        void eraseParam(const std::string& key) {
            for (auto it = params.begin(); it != params.end(); it++) {
                if (it->first == key)
                    it = params.erase(it);
                else
                    it++;
            }
        }

        std::string getString() {
            std::stringstream ss;
            bool first = true;
            for (auto& e : params) {
                if (first) first = false;
                else ss << " ";
                if (e.first != "command")
                    ss << e.first << "=";
                ss << e.second;
            }
            return ss.str();
        }
    };
}


#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace gfn {
    class Command {
    public:
        std::vector<std::pair<std::string, std::string>> params;

        Command() = default;

        Command(const std::string& command) {
            std::stringstream ss;
            ss.str(command);
            std::vector<std::string> v;
            std::string temp;
            while (ss >> temp)
                v.push_back(temp);
            if (v.empty())
                return;
            if ((*v.begin())[0] != '-')
                params.emplace_back("command", "");
            for (auto& s : v) {
                if (s[0] == '-')
                    params.emplace_back(s, "");
                else {
                    if ((*params.rbegin()).second.empty())
                        (*params.rbegin()).second.append(s);
                    else
                        (*params.rbegin()).second.append(" " + s);
                }
            }
        }

        std::string getParamValue(const std::string& paramKey) {
            for (auto& e : params) {
                if (e.first == paramKey)
                    return e.second;
            }
            return "";
        }

        std::string& getParamValueRef(const std::string& paramKey) {
            for (auto& e : params) {
                if (e.first == paramKey)
                    return e.second;
            }
        }

        bool getFlag(const std::string& paramKey) {
            return !getParamValue(paramKey).empty();
        }

        void newParam(const std::string& key, const std::string& value) {
            params.emplace_back(key, value);
        }

        void appendParam(const std::string& key, const std::string& value) {
            if (getParamValue(key).empty())
                params.emplace_back(key, value);
            else
                getParamValueRef(key).append(" " + value);
        }

        std::string getString() {
            std::stringstream ss;
            bool first = true;
            for (auto& e : params) {
                if (e.first != "command") {
                    if (first)
                        first = false;
                    else
                        ss << " ";
                    ss << e.first;
                }
                if (first)
                    first = false;
                else
                    ss << " ";
                ss << e.second;
            }
            return ss.str();
        }
    };
}


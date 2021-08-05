#include "Command.h"

namespace gfn {
    Command::Command() = default;

    Command::Command(const std::string& command) {
        params.emplace_back("command", "");
        bool commandState = true;
        bool quotesState = false;
        bool paramKeyState = false;
        bool paramValueState = false;

        for (auto& c : command) {
            if (c == '"') {
                quotesState = !quotesState;
                continue;
            }
            if (quotesState) {
                if (paramKeyState)
                    params[params.size() - 1].first.push_back(c);
                else if (paramValueState | commandState)
                    params[params.size() - 1].second.push_back(c);
                continue;
            }
            if (c == ' ') { // space not in quotes
                paramKeyState = false;
                paramValueState = false;
            }
            if (c == '-' && !paramKeyState && !paramValueState) {
                if (commandState) {
                    commandState = false;
                    // remove trailing spaces
                    while (params[params.size() - 1].second[params[params.size() - 1].second.size() - 1] == ' ')
                        params[params.size() - 1].second.erase(params[params.size() - 1].second.end() - 1);
                }
                paramKeyState = true;
                while (params[params.size() - 1].first[params[params.size() - 1].first.size() - 1] == ' ')
                    params[params.size() - 1].first.erase(params[params.size() - 1].first.end() - 1);
                while (params[params.size() - 1].second[params[params.size() - 1].second.size() - 1] == ' ')
                    params[params.size() - 1].second.erase(params[params.size() - 1].second.end() - 1);
                params.emplace_back("-", "");
                continue;
            }
            if (commandState) {
                if (params[params.size() - 1].second.empty() && c == ' ')
                    continue;
                params[params.size() - 1].second.push_back(c);
                continue;
            }
            if (c == '=') {
                paramKeyState = false;
                paramValueState = true;
                continue;
            }
            if (paramKeyState)
                params[params.size() - 1].first.push_back(c);
            else if (paramValueState | commandState)
                params[params.size() - 1].second.push_back(c);
        }
    }

    std::string Command::getParamValue(const std::string& paramKey) {
        for (auto& e : params) {
            if (e.first == paramKey)
                return e.second;
        }
        return "";
    }

    bool Command::getFlag(const std::string& paramKey) {
        return !getParamValue(paramKey).empty();
    }

    void Command::newParam(const std::string& key, const std::string& value) {
        params.emplace_back(key, value);
    }

    void Command::eraseParam(const std::string& key) {
        for (auto it = params.begin(); it != params.end(); it++) {
            if (it->first == key)
                it = params.erase(it);
            else
                it++;
        }
    }

    std::string Command::getString() {
        std::stringstream ss;
        bool first = true;
        for (auto& e : params) {
            if (first) first = false;
            else ss << " ";
            if (e.first != "command") {
                if (e.first.find(' ') != std::basic_string<char>::npos)
                    ss << "\"" << e.first << "\"=";
                else
                    ss << e.first << "=";
            }
            if (e.second.find(' ') != std::basic_string<char>::npos)
                ss << "\"" << e.second << "\"";
            else
                ss << e.second;
        }
        return ss.str();
    }
}
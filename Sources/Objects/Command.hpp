#pragma once

#include <string>
#include <vector>
#include <queue>
#include <regex>
#include <sstream>

namespace gfn {
    class Command {
    public:
        std::vector<std::pair<std::string, std::string>> params;

        Command() = default;

        Command(const std::string& command) {
            /*
             * Command parser
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

            /*std::cerr << "<BEGIN>\n";
            for (auto& p : params) {
                std::cerr << "[" << p.first << "] -> [" << p.second << "]\n";
            }
            std::cerr << "<END>\n";*/
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
                if (e.first != "command") {
                    if (e.second.find(' ') != std::basic_string<char>::npos)
                        ss << "=\"" << e.first << "\"";
                    else
                        ss << "=" << e.first;
                }
                if (e.second.find(' ') != std::basic_string<char>::npos)
                    ss << "\"" << e.second << "\"";
                else
                    ss << e.second;
            }
            return ss.str();
        }
    };
}


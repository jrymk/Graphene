#pragma once

#include <unordered_map>
#include <Objects/Vec2f.hpp>
#include <Objects/Uuid.hpp>
#include <Objects/Color.hpp>
#include <Objects/Command.hpp>
#include <imgui_internal.h>
#include <utility>
#include <sstream>
#include <iomanip>
#include <json.hpp>
#include <binn.h>

// these are containers that can work with string keys and string values, for loading up save files and configs from
// file or set and read from the command line

// get(): obtain the actual value
// getValueStr(): get the value in string form, for outputting to the console or display
// setValueStr(): set the value with a predefined format, mainly for console input
// serialize(): serialize the value into the given json entry (save value to json), for application configs
// deserialize(): deserialize the value from the given json entry (get value from json), for application configs
namespace gfn::parsables {
    class Uuid {
    public:
        std::string key;
        gfn::Uuid value;

        explicit Uuid(const std::string& key) : key(key) {}

        Uuid(const std::string& key, const gfn::Uuid& value) : key(key), value(value) {}

        gfn::Uuid& get() { return value; }

        void getValueStr(gfn::Command& output) {
            output.newParam("-value", value);
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            if (uuid::isUuid(value)) {
                value = _value;
                output.newParam("-successful", "true");
                getValueStr(output);
            } else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected UUID with format ********-****-****-****-************");
            }
            return;
        }

        void serialize(nlohmann::json& j) {
            j[key] = value;
        }

        void deserialize(nlohmann::json& j) {
            value = j[key].get<std::string>();
        }
    };

    class Bool {
    public:
        std::string key;
        bool value;

        Bool(const std::string& key) : key(key) {}

        Bool(const std::string& key, const bool& value) : key(key), value(value) {}

        bool& get() { return value; }

        void getValueStr(gfn::Command& output) {
            output.newParam("-value", (value ? "true" : "false"));
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            if (_value == "true")
                value = true;
            else if (_value == "false")
                value = false;
            else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected true or false");
                return;
            }
            output.newParam("-successful", "true");
            getValueStr(output);
            return;
        }

        void serialize(nlohmann::json& j) {
            j[key] = value;
        }

        void deserialize(nlohmann::json& j) {
            value = j[key].get<bool>();
        }
    };

    class Int {
    public:
        std::string key;
        int value;

        Int(const std::string& key) : key(key) {}

        Int(const std::string& key, const int& value) : key(key), value(value) {}

        int& get() { return value; }

        void getValueStr(gfn::Command& output) {
            output.newParam("-value", std::to_string(value));
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            int x;
            if (sscanf(_value.c_str(), "%d", &x) == 1) {
                value = x;
                output.newParam("-successful", "true");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "+%d", &x) == 1) {
                value += x;
                output.newParam("-successful", "true");
                getValueStr(output);
                return;
            } else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected integer number. "
                                        "To increment the value, add a + sign in the very beginning.");
                return;
            }
        }

        void serialize(nlohmann::json& j) {
            j[key] = value;
        }

        void deserialize(nlohmann::json& j) {
            value = j[key].get<int>();
        }
    };

    class Double {
    public:
        std::string key;
        double value;

        Double(const std::string& key) : key(key) {}

        Double(const std::string& key, const double& value) : key(key), value(value) {}

        double& get() { return value; }

        void getValueStr(gfn::Command& output) {
            output.newParam("-value", std::to_string(value));
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            double x;
            if (sscanf(_value.c_str(), "%lf", &x) == 1) {
                value = x;
                output.newParam("-successful", "true");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "+%lf", &x) == 1) {
                value += x;
                output.newParam("-successful", "true");
                getValueStr(output);
                return;
            } else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected floating point number. "
                                        "To increment the value, add a + sign in the very beginning.");
                return;
            }
        }

        void serialize(nlohmann::json& j) {
            j[key] = value;
        }

        void deserialize(nlohmann::json& j) {
            value = j[key].get<double>();
        }
    };

    class Vec2f {
    public:
        std::string key;
        gfn::Vec2f value;

        Vec2f(const std::string& key) : key(key) {}

        Vec2f(const std::string& key, const gfn::Vec2f& value) : key(key), value(value) {}

        gfn::Vec2f& get() { return value; }

        void getValueStr(gfn::Command& output) {
            output.newParam("-value", "(" + std::to_string(value.x) + "," + std::to_string(value.y) + ")");
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            double x, y;
            if (sscanf(_value.c_str(), "(%lf,%lf)", &x, &y) == 2) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.x = x;
                value.y = y;
                return;
            } else if (sscanf(_value.c_str(), "x(%lf)", &x) == 1) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.x = x;
                return;
            } else if (sscanf(_value.c_str(), "y(%lf)", &y) == 1) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.y = y;
                return;
            } else if (sscanf(_value.c_str(), "+(%lf,%lf)", &x, &y) == 2) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.x += x;
                value.y += y;
                return;
            } else if (sscanf(_value.c_str(), "+x(%lf)", &x) == 1) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.x += x;
                return;
            } else if (sscanf(_value.c_str(), "+y(%lf)", &y) == 1) {
                output.newParam("-successful", "true");
                getValueStr(output);
                value.y += y;
                return;
            } else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix", "Expected two float numbers in format (x,y) or "
                                        "x(x) or y(y). To increment the value, add a + sign in the very beginning.");
                return;
            }
        }

        void serialize(nlohmann::json& j) {
            j[key]["x"] = value.x;
            j[key]["y"] = value.y;
        }

        void deserialize(nlohmann::json& j) {
            value.x = j[key]["x"].get<double>();
            value.y = j[key]["y"].get<double>();
        }
    };

    class Color {
    public:
        std::string key;
        ImU32 value;

        Color(const std::string& key) : key(key) {}

        Color(const std::string& key, const ImU32& value) : key(key), value(value) {}

        ImU32& get() { return value; }

        void getValueStr(gfn::Command& output) {
            ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
            output.newParam("-value", "(" +
                                      std::to_string(int(rgba.x * 255.0f + 0.5f)) + "," +
                                      std::to_string(int(rgba.y * 255.0f + 0.5f)) + "," +
                                      std::to_string(int(rgba.z * 255.0f + 0.5f)) + "," +
                                      std::to_string(int(rgba.w * 255.0f + 0.5f)) + ")");
        }

        void setValueStr(const std::string& _value, gfn::Command& output) {
            float x, y, z, w;
            uint32_t u32;
            if (sscanf(_value.c_str(), "rgba(%f,%f,%f,%f)", &x, &y, &z, &w) == 4) {
                x = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                y = std::min(255.0f, std::max(0.0f, y)) / 255.0f;
                z = std::min(255.0f, std::max(0.0f, z)) / 255.0f;
                w = std::min(255.0f, std::max(0.0f, w)) / 255.0f;
                value = ImGui::ColorConvertFloat4ToU32(ImVec4(x, y, z, w));
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "rgba");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "rgb(%f,%f,%f)", &x, &y, &z) == 3) {
                x = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                y = std::min(255.0f, std::max(0.0f, y)) / 255.0f;
                z = std::min(255.0f, std::max(0.0f, z)) / 255.0f;
                value = ImGui::ColorConvertFloat4ToU32(ImVec4(x, y, z, ImGui::ColorConvertU32ToFloat4(value).w));
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "rgb");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "hsva(%f,%f,%f,%f)", &x, &y, &z, &w) == 4) {
                float tH = std::min(360.0f, std::max(0.0f, x)) / 360.0f;
                float tS = std::min(100.0f, std::max(0.0f, y)) / 100.0f;
                float tV = std::min(100.0f, std::max(0.0f, z)) / 100.0f;
                float tA = std::min(255.0f, std::max(0.0f, w)) / 255.0f;
                ImVec4 rgba(1.0f, 0.0f, 1.0f, tA);
                ImGui::ColorConvertHSVtoRGB(tH, tS, tV, rgba.x, rgba.y, rgba.z);
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "hsva");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "hsv(%f,%f,%f)", &x, &y, &z) == 3) {
                float tH = std::min(360.0f, std::max(0.0f, x)) / 360.0f;
                float tS = std::min(100.0f, std::max(0.0f, y)) / 100.0f;
                float tV = std::min(100.0f, std::max(0.0f, z)) / 100.0f;
                ImVec4 rgba(1.0f, 0.0f, 1.0f, ImGui::ColorConvertU32ToFloat4(value).w);
                ImGui::ColorConvertHSVtoRGB(tH, tS, tV, rgba.x, rgba.y, rgba.z);
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "hsv");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "d%u", &u32) == 1) {
                value = u32;
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "dec");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "%x", &u32) == 1) {
                value = u32;
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "hex");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "r(%f)", &x) == 1) {
                float r = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                rgba.x = r;
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "r");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "g(%f)", &x) == 1) {
                float g = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                rgba.y = g;
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "g");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "b(%f)", &x) == 1) {
                float b = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                rgba.z = b;
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "b");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "a(%f)", &x) == 1) {
                float a = std::min(255.0f, std::max(0.0f, x)) / 255.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                rgba.w = a;
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "a");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "h(%f)", &x) == 1) {
                float h = std::min(360.0f, std::max(0.0f, x)) / 360.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                ImVec4 hsva;
                ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, hsva.x, hsva.y, hsva.z);
                hsva.x = h;
                ImGui::ColorConvertHSVtoRGB(hsva.x, hsva.y, hsva.z, rgba.x, rgba.y, rgba.z);
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "h");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "s(%f)", &x) == 1) {
                float s = std::min(100.0f, std::max(0.0f, x)) / 100.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                ImVec4 hsva;
                ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, hsva.x, hsva.y, hsva.z);
                hsva.y = s;
                ImGui::ColorConvertHSVtoRGB(hsva.x, hsva.y, hsva.z, rgba.x, rgba.y, rgba.z);
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "s");
                getValueStr(output);
                return;
            } else if (sscanf(_value.c_str(), "v(%f)", &x) == 1) {
                float v = std::min(100.0f, std::max(0.0f, x)) / 100.0f;
                ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
                ImVec4 hsva;
                ImGui::ColorConvertRGBtoHSV(rgba.x, rgba.y, rgba.z, hsva.x, hsva.y, hsva.z);
                hsva.z = v;
                ImGui::ColorConvertHSVtoRGB(hsva.x, hsva.y, hsva.z, rgba.x, rgba.y, rgba.z);
                value = ImGui::ColorConvertFloat4ToU32(rgba);
                output.newParam("-successful", "true");
                output.newParam("-parse-mode", "v");
                getValueStr(output);
                return;
            } else {
                output.newParam("-successful", "false");
                output.newParam("-error", "ILLEGAL_VALUE");
                output.newParam("-fix",
                                "Expected rgba(r,g,b,a), rgb(r,g,b), hsva(h,s,v,a), hsv(h,s,v), "
                                "r(r), g(g), b(b), a(a), h(h), s(s), v(v), hexadecimal or decimal. "
                                "Floating numbers accepted for r, g, b, h, s, v, a. "
                                "r, g, b, a ranging from 0 to 255, h from 0 to 360 (in degrees), "
                                "s and v from 0 to 100 (in percentage). Alternaltively, do for example 6ce813FF"
                                " or d1827148799");
                return;
            }
        }

        void serialize(nlohmann::json& j) {
            std::stringstream ss;
            std::string hexStr;
            ss << value;
            ss >> std::setw(8) >> std::setfill('0') >> std::hex >> hexStr;
            j[key] = hexStr;
        }

        void deserialize(nlohmann::json& j) {
            std::stringstream ss;
            ss << std::hex << j[key].get<std::string>();
            ss >> value;
        }
    };

}
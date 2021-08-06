#include "Serializable.h"

namespace gfn::serializable {
    Uuid::Uuid(std::string key) : key(std::move(key)), value() {}

    Uuid::Uuid(std::string key, gfn::Uuid value) : key(std::move(key)), value(std::move(value)) {}

    gfn::Uuid& Uuid::get() { return value; }

    void Uuid::getValueStr(gfn::Args& output) const {
        output.newParam("-value", value);
    }

    void Uuid::setValueStr(const std::string& _value, gfn::Args& output) {
        if (isUuid(value)) {
            value = _value;
            output.newParam("-successful", "true");
            getValueStr(output);
        } else {
            output.newParam("-successful", "false");
            output.newParam("-error", "ILLEGAL_VALUE");
            output.newParam("-fix", "Expected UUID with format ********-****-****-****-************");
        }
    }

    void Uuid::serialize(nlohmann::json& j) {
        j[key] = value;
    }

    void Uuid::deserialize(nlohmann::json& j) {
        value = j[key].get<std::string>();
    }


    Bool::Bool(std::string key) : key(std::move(key)), value(false) {}

    Bool::Bool(std::string key, const bool& value) : key(std::move(key)), value(value) {}

    bool& Bool::get() { return value; }

    void Bool::getValueStr(gfn::Args& output) const {
        output.newParam("-value", (value ? "true" : "false"));
    }

    void Bool::setValueStr(const std::string& _value, gfn::Args& output) {
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
    }

    void Bool::serialize(nlohmann::json& j) {
        j[key] = value;
    }

    void Bool::deserialize(nlohmann::json& j) {
        value = j[key].get<bool>();
    }


    Int::Int(std::string key) : key(std::move(key)), value(0) {}

    Int::Int(std::string key, const int& value) : key(std::move(key)), value(value) {}

    int& Int::get() { return value; }

    void Int::getValueStr(gfn::Args& output) const {
        output.newParam("-value", std::to_string(value));
    }

    void Int::setValueStr(const std::string& _value, gfn::Args& output) {
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

    void Int::serialize(nlohmann::json& j) {
        j[key] = value;
    }

    void Int::deserialize(nlohmann::json& j) {
        value = j[key].get<int>();
    }


    Double::Double(std::string key) : key(std::move(key)), value(0.0) {}

    Double::Double(std::string key, const double& value) : key(std::move(key)), value(value) {}

    double& Double::get() { return value; }

    void Double::getValueStr(gfn::Args& output) const {
        output.newParam("-value", std::to_string(value));
    }

    void Double::setValueStr(const std::string& _value, gfn::Args& output) {
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

    void Double::serialize(nlohmann::json& j) {
        j[key] = value;
    }

    void Double::deserialize(nlohmann::json& j) {
        value = j[key].get<double>();
    }


    Vec2f::Vec2f(std::string key) : key(std::move(key)), value(gfn::Vec2(0.0, 0.0)) {}

    Vec2f::Vec2f(std::string key, const gfn::Vec2& value) : key(std::move(key)), value(value) {}

    gfn::Vec2& Vec2f::get() { return value; }

    void Vec2f::getValueStr(gfn::Args& output) const {
        output.newParam("-value", "(" + std::to_string(value.x) + "," + std::to_string(value.y) + ")");
    }

    void Vec2f::setValueStr(const std::string& _value, gfn::Args& output) {
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

    void Vec2f::serialize(nlohmann::json& j) {
        j[key]["x"] = value.x;
        j[key]["y"] = value.y;
    }

    void Vec2f::deserialize(nlohmann::json& j) {
        value.x = j[key]["x"].get<double>();
        value.y = j[key]["y"].get<double>();
    }


    Color::Color(std::string key) : key(std::move(key)), value(IM_COL32(255, 0, 255, 255)) {}

    Color::Color(std::string key, const ImU32& value) : key(std::move(key)), value(value) {}

    ImU32& Color::get() { return value; }

    void Color::getValueStr(gfn::Args& output) const {
        ImVec4 rgba = ImGui::ColorConvertU32ToFloat4(value);
        output.newParam("-value", "(" +
                                  std::to_string(int(rgba.x * 255.0f + 0.5f)) + "," +
                                  std::to_string(int(rgba.y * 255.0f + 0.5f)) + "," +
                                  std::to_string(int(rgba.z * 255.0f + 0.5f)) + "," +
                                  std::to_string(int(rgba.w * 255.0f + 0.5f)) + ")");
    }

    void Color::setValueStr(const std::string& _value, gfn::Args& output) {
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

    void Color::serialize(nlohmann::json& j) const {
        std::stringstream ss;
        std::string hexStr;
        ss << value;
        ss >> std::setw(8) >> std::setfill('0') >> std::hex >> hexStr;
        j[key] = hexStr;
    }

    void Color::deserialize(nlohmann::json& j) {
        std::stringstream ss;
        ss << std::hex << j[key].get<std::string>();
        ss >> value;
    }
}
#ifndef PARSABLES_H_
#define PARSABLES_H_

#include <string>
#include <Objects/Command.h>
#include <Objects/Uuid.h>
#include <Objects/Vec2.h>
#include <json.hpp>
#include <imgui.h>

// these are containers that can work with string keys and string values, for loading up save files and configs from
// file or set and read from the command line

// get(): obtain the actual value
// getValueStr(): get the value in string form, for outputting to the console or display
// setValueStr(): set the value with a predefined format, mainly for console input
// serialize(): serialize the value into the given json entry (save value to json), for application configs
// deserialize(): deserialize the value from the given json entry (get value from json), for application configs
namespace gfn::serializable {
    class Uuid {
    public:
        std::string key;
        gfn::Uuid value;

        explicit Uuid(std::string key);

        Uuid(std::string key, gfn::Uuid value);

        gfn::Uuid& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);
    };

    class Bool {
    public:
        std::string key;
        bool value;

        Bool(std::string key);

        Bool(std::string key, const bool& value);

        bool& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);
    };

    class Int {
    public:
        std::string key;
        int value;

        Int(std::string key);

        Int(std::string key, const int& value);

        int& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);
    };

    class Double {
    public:
        std::string key;
        double value;

        Double(std::string key);

        Double(std::string key, const double& value);

        double& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);
    };

    class Vec2f {
    public:
        std::string key;
        gfn::Vec2 value;

        Vec2f(std::string key);

        Vec2f(std::string key, const gfn::Vec2& value);

        gfn::Vec2& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);
    };

    class Color {
    public:
        std::string key;
        ImU32 value;

        Color(std::string key);

        Color(std::string key, const ImU32& value);

        ImU32& get();

        void getValueStr(gfn::Command& output) const;

        void setValueStr(const std::string& _value, gfn::Command& output);

        void serialize(nlohmann::json& j) const;

        void deserialize(nlohmann::json& j);
    };

}

#endif
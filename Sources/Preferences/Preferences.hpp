#pragma once

#include <imgui.h>
#include <Preferences/KeyBind/KeyBind.hpp>
#include <Objects/Parsables.hpp>
#include <json.hpp>
#include <fstream>

namespace gfn::preferences {
    class Preferences {
    public:
        gfn::keybind::KeyBind* keyBind;

        double graphview_zoom_speed = 1.1;
        double graphview_smooth_pan_speed = 0.3; // 1.0: disable smoothing
        double graphview_smooth_zoom_speed = 0.3; // 1.0: disable smoothing
        float graphview_selection_tolerance = 5; // 0.0: must click on point
        float glow_size = 0.2f;
        float graph_view_lasso_selection_threshold = 5.0f;

        void serialize(nlohmann::json& j) {
            keyBind->serialize(j["Key Binds"]);
        }

        void deserialize(nlohmann::json& j) {
            keyBind->deserialize(j["Key Binds"]);
        }

        void saveToFile() {
            std::ofstream prefFile("preferences.json", std::ios::out);
            if (prefFile) {
                nlohmann::json j;
                serialize(j);

                auto jDump = j.dump(4, ' ', true);
                prefFile << jDump;
                prefFile.close();
                std::cout << "Saved preferences.json (Output size: " << jDump.size() << " bytes)\n";
            }
        }

        void loadFromFile() {
            std::ifstream prefFile("preferences.json", std::ios::in);
            if (prefFile) {
                nlohmann::json j = nlohmann::json::parse(prefFile);
                //std::cout << j.dump(4, ' ', true) << "\n";
                deserialize(j);
            }
        }
    };
}
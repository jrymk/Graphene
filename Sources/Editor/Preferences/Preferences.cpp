#include "Preferences.h"
#include <iostream>
#include <fstream>

namespace gfn {
    void Preferences::serialize(nlohmann::json& j) {
        bindings.serialize(j["Key Binds"]);

        nlohmann::json gfxOptions;
        gfxOptions["Default font"] = graphics_default_font;
        gfxOptions["Font size"] = graphics_font_size;
        gfxOptions["Launch with window maximized"] = graphics_launch_maximized;
        gfxOptions["Launch with window width"] = graphics_launch_window_width;
        gfxOptions["Launch with window height"] = graphics_launch_window_height;
        gfxOptions["Enable vertical sync"] = graphics_vertical_sync;
        gfxOptions["Anti-aliasing samples"] = graphics_antialiasing_samples;
        gfxOptions["Enable multiple viewports"] = graphics_multiple_viewports;
        j["Graphics Options"] = gfxOptions;

        nlohmann::json gvOptions;
        gvOptions["Zoom speed"] = graphview_zoom_speed;
        gvOptions["Selection tolerance"] = graphview_selection_tolerance;
        gvOptions["Selection highlight glow size"] = glow_size;
        j["Graph View Options"] = gvOptions;
    }

    void Preferences::deserialize(nlohmann::json& j) {
        bindings.deserialize(j["Key Binds"]);

        nlohmann::json gfxOptions = j["Graphics Options"];
        graphics_default_font = gfxOptions["Default font"];
        graphics_font_size = gfxOptions["Font size"];
        graphics_launch_maximized = gfxOptions["Launch with window maximized"];
        graphics_launch_window_width = gfxOptions["Launch with window width"];
        graphics_launch_window_height = gfxOptions["Launch with window height"];
        graphics_vertical_sync = gfxOptions["Enable vertical sync"];
        graphics_antialiasing_samples = gfxOptions["Anti-aliasing samples"];
        graphics_multiple_viewports = gfxOptions["Enable multiple viewports"];

        nlohmann::json gvOptions = j["Graph View Options"];
        graphview_zoom_speed = gvOptions["Zoom speed"];
        graphview_selection_tolerance = gvOptions["Selection tolerance"];
        glow_size = gvOptions["Selection highlight glow size"];
    }

    void Preferences::saveToFile() {
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

    void Preferences::loadFromFile() {
        std::ifstream prefFile("preferences.json", std::ios::in);
        if (prefFile) {
            nlohmann::json j = nlohmann::json::parse(prefFile);
            //std::cout << j.dump(4, ' ', true) << "\n";
            deserialize(j);
        }
    }

    void Preferences::checkSave() {
        if (bindings.wantSaveBindings) {
            saveToFile();
            bindings.wantSaveBindings = false;
        }
    }
}
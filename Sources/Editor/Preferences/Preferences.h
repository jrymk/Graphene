#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <Editor/Preferences/KeyBindings/KeyBindings.h>
#include <json.hpp>

namespace gfn {
    class Preferences {
    public:
        std::string graphics_default_font = ".\\Barlow-Regular.ttf";
        float graphics_font_size = 16.0f;
        bool graphics_launch_maximized = false;
        int graphics_launch_window_width = 1280;
        int graphics_launch_window_height = 720;
        bool graphics_vertical_sync = true;
        int graphics_antialiasing_samples = 4;
        bool graphics_multiple_viewports = true;

        gfn::Bindings bindings;

        double graphview_zoom_speed = 1.1;
        double graphview_smooth_pan_speed = 0.3; // 1.0: disable smoothing
        double graphview_smooth_zoom_speed = 0.3; // 1.0: disable smoothing
        float graphview_selection_tolerance = 5; // 0.0: must click on point
        float glow_size = 0.2f;
        float graph_view_lasso_selection_threshold = 5.0f;

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);

        void saveToFile();

        void loadFromFile();

        void checkSave();
    };
}

#endif
#pragma once

#include <imgui.h>
#include <Preferences/KeyBind.hpp>

namespace gfn::preferences {
    class Preferences {
    private:
        gfn::keybind::KeyBind keyBind;

    public:
        gfn::keybind::KeyBind& getKeyBind() {
            return keyBind;
        }

        double graphview_zoom_speed = 1.1;
        double graphview_smooth_pan_speed = 0.3; // 1.0: disable smoothing
        double graphview_smooth_zoom_speed = 0.3; // 1.0: disable smoothing
        float graphview_selection_tolerance = 5; // 0.0: must click on point


    };
}
#ifndef THEME_H
#define THEME_H

#include <string>
#include <Editor/Preferences/Preferences.h>

namespace gfn {
    void setTheme(Preferences* prefs);

    void setStyle();

    void setColorLight();

    void setColorDark();

    enum Hue { // also saturation...
        HUE_DEFAULT = 0b00,
        HUE_CONTRAST = 0b10,
        HUE_TRANSPARENT = 0b01,
        HUE_RED = 0b00100,
        HUE_ORANGE = 0b01000,
        HUE_YELLOW = 0b01100,
        HUE_GREEN = 0b10000,
        HUE_CYAN = 0b10100,
        HUE_BLUE = 0b11000,
        HUE_PURPLE = 0b11100,
        HUE_RED_CONTRAST = 0b00110,
        HUE_ORANGE_CONTRAST = 0b01010,
        HUE_YELLOW_CONTRAST = 0b01110,
        HUE_GREEN_CONTRAST = 0b10010,
        HUE_CYAN_CONTRAST = 0b10110,
        HUE_BLUE_CONTRAST = 0b11010,
        HUE_PURPLE_CONTRAST = 0b11110,
    };

    void text(const std::string& label, Hue text);

    bool button(const std::string& label, Hue text, Hue fill, bool border, float width, float height, bool small = true);
}

#endif
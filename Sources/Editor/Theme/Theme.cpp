#include "Theme.h"

#include <imgui.h>

namespace gfn {
    void setTheme(Preferences* prefs) {
        setStyle();
        if (prefs->themes_use_dark_theme)
            setColorDark();
        else
            setColorLight();
    }

    void text(const std::string& label, Hue text) {
        ImVec4 textCol = ImGui::GetStyleColorVec4(ImGuiCol_Text);

        ImVec4 textColHSV = textCol;

        ImGui::ColorConvertRGBtoHSV(textCol.x, textCol.y, textCol.z, textColHSV.x, textColHSV.y, textColHSV.z);

        if (text & 0b10)  // contrast
            textColHSV.z = 0.5f - (0.5f - textColHSV.z);

        if (text & 0b01) { // transparent
            textColHSV.w = 0.0f;
        }

        float hue = 0.0;
        float saturation = 0.0;
        if ((text & 0b00100) == 0b00100) { // red
            hue = 0.01666666666666666666666666666667;
            saturation = 0.74;
        }

        if ((text & 0b01000) == 0b01000) { // orange
            hue = 0.07777777777777777777777777777778;
            saturation = 0.85;
        }

        if ((text & 0b01100) == 0b01100) { // yellow
            hue = 0.13333333333333333333333333333333;
            saturation = 0.94;
        }

        if ((text & 0b10000) == 0b10000) { // green
            hue = 0.40277777777777777777777777777778;
            saturation = 0.77;
        }

        if ((text & 0b10100) == 0b10100) { // cyan
            hue = 0.46666666666666666666666666666667;
            saturation = 0.86;
        }

        if ((text & 0b11000) == 0b11000) { // blue
            hue = 0.56666666666666666666666666666667;
            saturation = 0.76;
        }

        if ((text & 0b11100) == 0b11100) { // purple
            hue = 0.78611111111111111111111111111111;
            saturation = 0.51;
        }

        textColHSV.x = hue;

        textColHSV.y = saturation * 0.5f;

        ImGui::ColorConvertHSVtoRGB(textColHSV.x, textColHSV.y, textColHSV.z, textCol.x, textCol.y, textCol.z);

        ImGui::PushStyleColor(ImGuiCol_Text, textCol);

        ImGui::Text(label.c_str());

        ImGui::PopStyleColor(1);
    }

    void button(const std::string& label, Hue text, Hue fill, bool border, float width, float height, bool small) {
        if (!border)
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);

        ImVec4 textCol = ImGui::GetStyleColorVec4(ImGuiCol_Text);

        ImVec4 textColHSV = textCol;

        ImGui::ColorConvertRGBtoHSV(textCol.x, textCol.y, textCol.z, textColHSV.x, textColHSV.y, textColHSV.z);

        if (text & 0b10)  // contrast
            textColHSV.z = 0.5f + (0.5f - textColHSV.z);

        if (text & 0b01) { // transparent
            textColHSV.w = 0.0f;
        }

        float hue = 0.0;
        float saturation = 0.0;
        if ((text & 0b00100) == 0b00100) { // red
            hue = 0.01666666666666666666666666666667;
            saturation = 0.74;
        }

        if ((text & 0b01000) == 0b01000) { // orange
            hue = 0.07777777777777777777777777777778;
            saturation = 0.85;
        }

        if ((text & 0b01100) == 0b01100) { // yellow
            hue = 0.13333333333333333333333333333333;
            saturation = 0.94;
        }

        if ((text & 0b10000) == 0b10000) { // green
            hue = 0.40277777777777777777777777777778;
            saturation = 0.77;
        }

        if ((text & 0b10100) == 0b10100) { // cyan
            hue = 0.46666666666666666666666666666667;
            saturation = 0.86;
        }

        if ((text & 0b11000) == 0b11000) { // blue
            hue = 0.56666666666666666666666666666667;
            saturation = 0.76;
        }

        if ((text & 0b11100) == 0b11100) { // purple
            hue = 0.78611111111111111111111111111111;
            saturation = 0.51;
        }

        textColHSV.x = hue;

        textColHSV.y = saturation * 0.5f;

        ImGui::ColorConvertHSVtoRGB(textColHSV.x, textColHSV.y, textColHSV.z, textCol.x, textCol.y, textCol.z);

        ImGui::PushStyleColor(ImGuiCol_Text, textCol);

        ImVec4 borderCol = ImGui::GetStyleColorVec4(ImGuiCol_Border);
        ImVec4 buttonCol = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImVec4 buttonColHovered = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
        ImVec4 buttonColActive = ImGui::GetStyleColorVec4(ImGuiCol_ButtonActive);

        ImVec4 borderColHSV = borderCol;
        ImVec4 buttonColHSV = buttonCol;
        ImVec4 buttonColHoveredHSV = buttonColHovered;
        ImVec4 buttonColActiveHSV = buttonColActive;

        ImGui::ColorConvertRGBtoHSV(borderCol.x, borderCol.y, borderCol.z, borderColHSV.x, borderColHSV.y, borderColHSV.z);
        ImGui::ColorConvertRGBtoHSV(buttonCol.x, buttonCol.y, buttonCol.z, buttonColHSV.x, buttonColHSV.y, buttonColHSV.z);
        ImGui::ColorConvertRGBtoHSV(buttonColHovered.x, buttonColHovered.y, buttonColHovered.z,
                                    buttonColHoveredHSV.x, buttonColHoveredHSV.y, buttonColHoveredHSV.z);
        ImGui::ColorConvertRGBtoHSV(buttonColActive.x, buttonColActive.y, buttonColActive.z,
                                    buttonColActiveHSV.x, buttonColActiveHSV.y, buttonColActiveHSV.z);

        if (fill & 0b10) { // contrast
            borderColHSV.z = 0.5f + (0.5f - borderColHSV.z);
            buttonColHSV.z = 0.5f + (0.5f - buttonColHSV.z);
            buttonColHoveredHSV.z = 0.5f + (0.5f - buttonColHoveredHSV.z);
            buttonColActiveHSV.z = 0.5f + (0.5f - buttonColActiveHSV.z);
        }

        if (fill & 0b01) { // transparent
            borderColHSV.w = 0.0f;
            buttonColHSV.w = 0.0f;
            buttonColHoveredHSV.w = 0.0f;
            buttonColActiveHSV.w = 0.0f;
        }

        hue = 0.0;
        saturation = 0.0;
        if ((fill & 0b00100) == 0b00100) { // red
            hue = 0.01666666666666666666666666666667;
            saturation = 0.74;
        }

        if ((fill & 0b01000) == 0b01000) { // orange
            hue = 0.07777777777777777777777777777778;
            saturation = 0.85;
        }

        if ((fill & 0b01100) == 0b01100) { // yellow
            hue = 0.13333333333333333333333333333333;
            saturation = 0.94;
        }

        if ((fill & 0b10000) == 0b10000) { // green
            hue = 0.40277777777777777777777777777778;
            saturation = 0.77;
        }

        if ((fill & 0b10100) == 0b10100) { // cyan
            hue = 0.46666666666666666666666666666667;
            saturation = 0.86;
        }

        if ((fill & 0b11000) == 0b11000) { // blue
            hue = 0.56666666666666666666666666666667;
            saturation = 0.76;
        }

        if ((fill & 0b11100) == 0b11100) { // purple
            hue = 0.78611111111111111111111111111111;
            saturation = 0.51;
        }

        borderColHSV.x = hue;
        buttonColHSV.x = hue;
        buttonColHoveredHSV.x = hue;
        buttonColActiveHSV.x = hue;

        borderColHSV.y = saturation * 0.7f;
        buttonColHSV.y = saturation * 0.7f;
        buttonColHoveredHSV.y = saturation * 0.7f;
        buttonColActiveHSV.y = saturation * 0.7f;

        ImGui::ColorConvertHSVtoRGB(borderColHSV.x, borderColHSV.y, borderColHSV.z, borderCol.x, borderCol.y, borderCol.z);
        ImGui::ColorConvertHSVtoRGB(buttonColHSV.x, buttonColHSV.y, buttonColHSV.z, buttonCol.x, buttonCol.y, buttonCol.z);
        ImGui::ColorConvertHSVtoRGB(buttonColHoveredHSV.x, buttonColHoveredHSV.y, buttonColHoveredHSV.z,
                                    buttonColHovered.x, buttonColHovered.y, buttonColHovered.z);
        ImGui::ColorConvertHSVtoRGB(buttonColActiveHSV.x, buttonColActiveHSV.y, buttonColActiveHSV.z,
                                    buttonColActive.x, buttonColActive.y, buttonColActive.z);

        ImGui::PushStyleColor(ImGuiCol_Border, borderCol);
        ImGui::PushStyleColor(ImGuiCol_Button, buttonCol);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonColHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonColActive);

        if (small)
            ImGui::SmallButton(label.c_str());
        else
            ImGui::Button(label.c_str(), ImVec2(width, height));

        ImGui::PopStyleColor(4);
        ImGui::PopStyleColor(1);
        if (!border)
            ImGui::PopStyleVar();
    }
}
#pragma once

#include <imgui.h>

namespace gfn {
    struct Color {
        uint32_t color32;

        Color() {
            color32 = IM_COL32(255, 0, 255, 255);
        }

        static float normalize(int v) {
            return float(v) / 255.0f;
        }

        void setRGBA(int r, int g, int b, int a) {
            color32 = IM_COL32(r, g, b, a);
        }

        void setHSVA(int h, int s, int v, int a) {
            float r, g, b;
            ImGui::ColorConvertHSVtoRGB(normalize(h), normalize(s), normalize(v), r, g, b);
            color32 - ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, normalize(a)));
        }
    };
}

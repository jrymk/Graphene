#pragma once
#include <imgui.h>

namespace Gui {

    void AlignedText(ImVec2 origin, ImVec2 align, ImVec2 size, const char* label, ImFont* font, float scale, ImU32 col) {
        ImGui::PushFont(font);
        ImGui::SetWindowFontScale(scale);
        ImGui::PushStyleColor(ImGuiCol_Text, col);

        ImVec2 labelMinPos(origin.x - size.x * align.x, origin.y - size.y * align.y);
        ImVec2 labelMaxPos(origin.x + size.x * (1.0f - align.x), origin.y + size.y * (1.0f - align.y));

        ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, true);

        const ImRect bb(labelMinPos, labelMaxPos);
        ImGui::RenderTextClipped(labelMinPos, labelMaxPos, label, nullptr, &labelSize, align, &bb);

        ImGui::PopStyleColor(1);
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopFont();
    }

}
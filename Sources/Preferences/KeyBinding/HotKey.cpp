#include "KeyBinding.h"
#include <imgui.h>
#include <imgui_internal.h>

namespace gfn {
    HotKey::HotKey() {
        keyboard.reset();
        mouse = MOUSE_BUTTON_NONE;
    }

    HotKey HotKey::now() {
        HotKey hk;
        for (int i = 0; i < 512; i++) {
            if (ImGui::GetIO().KeysDown[i])
                hk.keyboard.set(i);
        }
        if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            hk.mouse |= MOUSE_BUTTON_LEFT;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
            hk.mouse |= MOUSE_BUTTON_RIGHT;
        if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
            hk.mouse |= MOUSE_BUTTON_MIDDLE;
        if (ImGui::IsMouseDown(3))
            hk.mouse |= MOUSE_BUTTON_FOUR;
        if (ImGui::IsMouseDown(4))
            hk.mouse |= MOUSE_BUTTON_FIVE;
        if (ImGui::GetIO().MouseWheel > FLT_MIN)
            hk.mouse |= MOUSE_SCROLL_UP;
        if (ImGui::GetIO().MouseWheel < -FLT_MIN)
            hk.mouse |= MOUSE_SCROLL_DOWN;
        if (ImGui::GetIO().MouseWheelH < -FLT_MIN)
            hk.mouse |= MOUSE_SCROLL_LEFT;
        if (ImGui::GetIO().MouseWheelH > FLT_MIN)
            hk.mouse |= MOUSE_SCROLL_RIGHT;
        return hk;
    }

    void HotKey::showKeysImGui(bool allowDelete) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !allowDelete);
        if (keyboard.none() && mouse == 0) {
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(223, 230, 233, 255));
            ImGui::SmallButton("No bindings");
            ImGui::PopStyleColor(1);
            ImGui::PopItemFlag();
            return;
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 234, 167, 255));
        for (int i = 340; i <= 348; i++) {
            if (keyboard[i]) {
                if (ImGui::SmallButton(gfn::HotKey::keyNames[i].c_str()) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(162, 155, 254, 255));
        for (int i = 256; i <= 339; i++) {
            if (keyboard[i]) {
                if (ImGui::SmallButton(gfn::HotKey::keyNames[i].c_str()) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(129, 236, 236, 255));
        for (int i = 0; i <= 255; i++) {
            if (keyboard[i]) {
                if (ImGui::SmallButton(gfn::HotKey::keyNames[i].c_str()) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(250, 177, 160, 255));
        if (mouse & MOUSE_BUTTON_LEFT) {
            if (ImGui::SmallButton("left mouse") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_RIGHT) {
            if (ImGui::SmallButton("right mouse") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_MIDDLE) {
            if (ImGui::SmallButton("middle mouse") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_FOUR) {
            if (ImGui::SmallButton("forth mouse") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_FIVE) {
            if (ImGui::SmallButton("fifth mouse") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_UP) {
            if (ImGui::SmallButton("scroll up") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_DOWN) {
            if (ImGui::SmallButton("scroll down") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_LEFT) {
            if (ImGui::SmallButton("scroll left") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_RIGHT) {
            if (ImGui::SmallButton("scroll right") && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(99, 110, 114, 255));
        for (int i = 349; i <= 511; i++) {
            if (keyboard[i]) {
                ImGui::SmallButton(gfn::HotKey::keyNames[i].c_str());
                ImGui::SameLine();
            }
        }
        ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(9, 132, 227, 255));
        ImGui::PopItemFlag();
        if (repeatStartMs >= 0) {
            ImGui::SmallButton("repeat");
            ImGui::SameLine();
        }
        ImGui::Text("");
        ImGui::PopStyleColor(7);
    }
}
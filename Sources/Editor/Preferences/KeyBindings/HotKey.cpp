#include "KeyBindings.h"
#include <Editor/Theme/Theme.h>
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
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        if (keyboard.none() && mouse == 0) {
            gfn::button("No bindings", HUE_RED, HUE_TRANSPARENT, false, 0, 0, true);
            ImGui::PopItemFlag();
            return;
        }
        ImGui::PopItemFlag();
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !allowDelete);
        for (int i = 340; i <= 348; i++) {
            if (keyboard[i]) {
                if (gfn::button(gfn::HotKey::keyNames[i], HUE_CONTRAST, HUE_YELLOW_CONTRAST, false, 0, 0, true) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        for (int i = 256; i <= 339; i++) {
            if (keyboard[i]) {
                if (gfn::button(gfn::HotKey::keyNames[i], HUE_CONTRAST, HUE_PURPLE_CONTRAST, false, 0, 0, true) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        for (int i = 0; i <= 255; i++) {
            if (keyboard[i]) {
                if (gfn::button(gfn::HotKey::keyNames[i], HUE_CONTRAST, HUE_CYAN_CONTRAST, false, 0, 0, true) && allowDelete)
                    keyboard.reset(i);
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
        }
        if (mouse & MOUSE_BUTTON_LEFT) {
            if (gfn::button("left mouse", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_RIGHT) {
            if (gfn::button("right mouse", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_MIDDLE) {
            if (gfn::button("middle mouse", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_FOUR) {
            if (gfn::button("fourth mouse", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_BUTTON_FIVE) {
            if (gfn::button("fifth mouse", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_UP) {
            if (gfn::button("scroll up", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_DOWN) {
            if (gfn::button("scroll down", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_LEFT) {
            if (gfn::button("scroll left", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        if (mouse & MOUSE_SCROLL_RIGHT) {
            if (gfn::button("scroll right", HUE_CONTRAST, HUE_RED_CONTRAST, false, 0, 0, true) && allowDelete)
                mouse &= ~MOUSE_BUTTON_LEFT;
            if (ImGui::IsItemHovered() && allowDelete)
                ImGui::SetTooltip("Remove");
            ImGui::SameLine();
        }
        for (int i = 349; i <= 511; i++) {
            if (keyboard[i]) {
                ImGui::SmallButton(gfn::HotKey::keyNames[i].c_str());
                ImGui::SameLine();
            }
        }
        if (repeatStartMs >= 0) {
            gfn::button("\ue040 repeat", HUE_BLUE, HUE_TRANSPARENT, true, 0, 0, true);
            ImGui::SameLine();
        }
        ImGui::PopItemFlag();
        ImGui::Text("");
    }
}
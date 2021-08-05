#include "KeyBinding.h"
#include <imgui.h>

namespace gfn {
    Bindings::Bindings() :
            _enrolling_hk(nullptr) {
        bindings.resize(actionNames.size());
    }

    std::vector<HotKey>& Bindings::getHotKeys(int action) {
        return bindings[action];
    }

    std::tuple<bool, float, HotKey> Bindings::isBindingActive(int action) {
        for (auto hk : bindings[action]) {
            if (!(hk.keyboard.none() && hk.mouse == 0)
                && (hk.mouse & gfn::HotKey::now().mouse) == hk.mouse
                && gfn::HotKey::now().keyboard == hk.keyboard) {
                float vel = FLT_MAX;
                if (hk.mouse & MOUSE_SCROLL_UP)
                    vel = std::min(vel, ImGui::GetIO().MouseWheel);
                if (hk.mouse & MOUSE_SCROLL_DOWN)
                    vel = std::min(vel, -ImGui::GetIO().MouseWheel);
                if (hk.mouse & MOUSE_SCROLL_LEFT)
                    vel = std::min(vel, ImGui::GetIO().MouseWheelH);
                if (hk.mouse & MOUSE_SCROLL_RIGHT)
                    vel = std::min(vel, -ImGui::GetIO().MouseWheelH);

                if (vel == FLT_MAX) // no velocity available
                    return {true, -1, hk};
                if (vel > 0)
                    return {true, vel, hk};
            }
        }
        return {false, -1, {}};
    }

    std::tuple<bool, float, HotKey> Bindings::isBindingActive(HotKey& hotkey) {
        if (!(hotkey.keyboard.none() && hotkey.mouse == 0)
            && (hotkey.mouse & gfn::HotKey::now().mouse) == hotkey.mouse
            && gfn::HotKey::now().keyboard == hotkey.keyboard) {
            float vel = FLT_MAX;
            if (hotkey.mouse & MOUSE_SCROLL_UP)
                vel = std::min(vel, ImGui::GetIO().MouseWheel);
            if (hotkey.mouse & MOUSE_SCROLL_DOWN)
                vel = std::min(vel, -ImGui::GetIO().MouseWheel);
            if (hotkey.mouse & MOUSE_SCROLL_LEFT)
                vel = std::min(vel, ImGui::GetIO().MouseWheelH);
            if (hotkey.mouse & MOUSE_SCROLL_RIGHT)
                vel = std::min(vel, -ImGui::GetIO().MouseWheelH);

            if (vel == FLT_MAX) // no velocity available
                return {true, -1, hotkey};
            if (vel > 0)
                return {true, vel, hotkey};
        }
        return {false, -1, {}};
    }

    void Bindings::serialize(nlohmann::json& j) {
        for (int i = 0; i < actionNames.size(); i++) {
            nlohmann::json hotkey = nlohmann::json::array();
            for (auto b : bindings[i]) {
                nlohmann::json binding;
                nlohmann::json keys = nlohmann::json::array();
                for (int k = 0; k < 512; k++) {
                    if (b.keyboard[k])
                        keys.push_back(HotKey::keyNames[k]);
                }
                if (b.mouse & MOUSE_BUTTON_LEFT)
                    keys.push_back("left_mouse");
                if (b.mouse & MOUSE_BUTTON_RIGHT)
                    keys.push_back("right_mouse");
                if (b.mouse & MOUSE_BUTTON_MIDDLE)
                    keys.push_back("middle_mouse");
                if (b.mouse & MOUSE_BUTTON_FOUR)
                    keys.push_back("forth_mouse");
                if (b.mouse & MOUSE_BUTTON_FIVE)
                    keys.push_back("fifth_mouse");
                if (b.mouse & MOUSE_SCROLL_UP)
                    keys.push_back("scroll_up");
                if (b.mouse & MOUSE_SCROLL_DOWN)
                    keys.push_back("scroll_down");
                if (b.mouse & MOUSE_SCROLL_LEFT)
                    keys.push_back("scroll_left");
                if (b.mouse & MOUSE_SCROLL_RIGHT)
                    keys.push_back("scroll_right");
                binding["Keys"] = keys;
                binding["Start repeat (ms)"] = b.repeatStartMs;
                binding["Repeat rate interval (ms)"] = b.repeatIntervalMs;
                hotkey.push_back(binding);
            }
            j[actionNames[i]] = hotkey;
        }
    }

    void Bindings::deserialize(nlohmann::json& j) {
        bindings.clear();
        bindings.resize(actionNames.size());
        for (auto&[action, bs] : j.items()) {
            for (auto& b : bs) {
                int actionId = -1;
                for (int i = 0; i < actionNames.size(); i++) {
                    if (actionNames[i] == action) {
                        actionId = i;
                        break;
                    }
                }
                if (actionId == -1)
                    continue;

                gfn::HotKey hk;
                for (auto& key : b["Keys"]) {
                    if (key == "left_mouse")
                        hk.mouse |= MOUSE_BUTTON_LEFT;
                    if (key == "right_mouse")
                        hk.mouse |= MOUSE_BUTTON_RIGHT;
                    if (key == "middle_mouse")
                        hk.mouse |= MOUSE_BUTTON_MIDDLE;
                    if (key == "forth_mouse")
                        hk.mouse |= MOUSE_BUTTON_FOUR;
                    if (key == "fifth_mouse")
                        hk.mouse |= MOUSE_BUTTON_FIVE;
                    if (key == "scroll_up")
                        hk.mouse |= MOUSE_SCROLL_UP;
                    if (key == "scroll_down")
                        hk.mouse |= MOUSE_SCROLL_DOWN;
                    if (key == "scroll_left")
                        hk.mouse |= MOUSE_SCROLL_LEFT;
                    if (key == "scroll_right")
                        hk.mouse |= MOUSE_SCROLL_RIGHT;

                    if (hk.mouse)
                        continue;
                    int keyCode = -1;
                    for (int i = 0; i < 512; i++) {
                        if (HotKey::keyNames[i] == key) {
                            keyCode = i;
                            break;
                        }
                    }
                    if (keyCode != -1)
                        hk.keyboard.set(keyCode);
                }
                hk.repeatStartMs = b["Start repeat (ms)"];
                hk.repeatIntervalMs = b["Repeat rate interval (ms)"];

                bindings[actionId].push_back(hk);
            }
        }
    }
}
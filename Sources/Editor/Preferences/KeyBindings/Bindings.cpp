#include "KeyBindings.h"
#include <imgui.h>

namespace gfn {
    Bindings::Bindings() :
            _enroll_target_hk(nullptr) {
        bindings.resize(actions.size());
        for (auto& b : bindings)
            b.resize(5);
    }

    std::vector<HotKey>& Bindings::getHotKeys(int action, int condition) {
        return bindings[action][condition];
    }

    std::tuple<bool, float, HotKey> Bindings::isBindingActive(int action, int condition) {
        for (auto hk : bindings[action][condition]) {
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
        std::string condNames[5] = {"Hover on void", "Hover on unselected vertex", "Hover on unselected edge",
                                    "Hover on selected vertex", "Hover on selected edge"};
        for (int a = 0; a < actions.size(); a++) {
            nlohmann::json condition;
            for (int c = 0; c < 5; c++) {
                std::string condName = condNames[c];
                if (a < nonCondActionStartsAt)
                    condName = "Always";
                for (auto h : bindings[a][c]) {
                    nlohmann::json hotkey;
                    nlohmann::json keys = nlohmann::json::array();
                    for (int k = 0; k < 512; k++) {
                        if (h.keyboard[k])
                            keys.push_back(HotKey::keyNames[k]);
                    }
                    if (h.mouse & MOUSE_BUTTON_LEFT)
                        keys.push_back("left_mouse");
                    if (h.mouse & MOUSE_BUTTON_RIGHT)
                        keys.push_back("right_mouse");
                    if (h.mouse & MOUSE_BUTTON_MIDDLE)
                        keys.push_back("middle_mouse");
                    if (h.mouse & MOUSE_BUTTON_FOUR)
                        keys.push_back("forth_mouse");
                    if (h.mouse & MOUSE_BUTTON_FIVE)
                        keys.push_back("fifth_mouse");
                    if (h.mouse & MOUSE_SCROLL_UP)
                        keys.push_back("scroll_up");
                    if (h.mouse & MOUSE_SCROLL_DOWN)
                        keys.push_back("scroll_down");
                    if (h.mouse & MOUSE_SCROLL_LEFT)
                        keys.push_back("scroll_left");
                    if (h.mouse & MOUSE_SCROLL_RIGHT)
                        keys.push_back("scroll_right");
                    hotkey["Keys"] = keys;
                    hotkey["Start repeat (ms)"] = h.repeatStartMs;
                    hotkey["Repeat rate interval (ms)"] = h.repeatIntervalMs;

                    condition[condName].push_back(hotkey);
                }
                if (a < nonCondActionStartsAt)
                    break;
            }
            j[actions[a].first] = condition;
        }
    }

    void Bindings::deserialize(nlohmann::json& j) {
        if (j.is_null())
            return;
        bindings.clear();
        bindings.resize(actions.size());
        for (auto& a :bindings)
            a.resize(5);
        std::string condNames[5] = {"Hover on void", "Hover on unselected vertex", "Hover on unselected edge",
                                    "Hover on selected vertex", "Hover on selected edge"};
        for (auto&[action, conds] : j.items()) {
            // find the coresponding action
            int actionId = -1;
            for (int i = 0; i < actions.size(); i++) {
                if (actions[i].first == action) {
                    actionId = i;
                    break;
                }
            }
            if (actionId == -1)
                continue;

            for (auto&[condName, hkList] : conds.items()) {
                int condId = -1;
                for (int i = 0; i < 5; i++)
                    if (condName == condNames[i])
                        condId = i;
                if (actionId < nonCondActionStartsAt)
                    condId = 0;
                if (condId == -1)
                    continue;
                for (auto& cHk : hkList) {
                    gfn::HotKey hk;
                    for (auto& key : cHk["Keys"]) {
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
                    hk.repeatStartMs = cHk["Start repeat (ms)"];
                    hk.repeatIntervalMs = cHk["Repeat rate interval (ms)"];

                    bindings[actionId][condId].push_back(hk);
                }
            }
        }
    }
}
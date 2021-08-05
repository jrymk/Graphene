#pragma once

#include <imgui.h>
#include <set>
#include <bitset>
#include <json.hpp>

#include "KeyNames.hpp"
#include "Actions.cpp"

namespace gfn::keybind {
    struct KeySet {
        std::bitset<512> keyFlags;

        KeySet() = default;

        void reset() { keyFlags.reset(); }

        void addKey(int key) { keyFlags.set(key); }

        void removeKey(int key) { keyFlags.reset(key); }
    };

    struct Binding {
        int mouseFlags = 0;
        KeySet keyFlags;
        int repeatStartMs = -1;
        int repeatIntervalMs = 100;

        Binding() {
            mouseFlags = MOUSE_BUTTON_NONE;
            keyFlags.reset();
        }

        Binding(int _mouseButtonFlags, KeySet _keyFlags) {
            mouseFlags = _mouseButtonFlags;
            keyFlags = _keyFlags;
        }

        std::string getKeyStr() {
            std::string keysStr;
            for (int k = 0; k < 512; k++) {
                if (keyFlags.keyFlags[k])
                    keysStr += (" " + keyNames[k] + " ");
            }
            if (mouseFlags & MOUSE_BUTTON_LEFT)
                keysStr += " left mouse ";
            if (mouseFlags & MOUSE_BUTTON_RIGHT)
                keysStr += " right mouse ";
            if (mouseFlags & MOUSE_BUTTON_MIDDLE)
                keysStr += " middle mouse ";
            if (mouseFlags & MOUSE_BUTTON_FOUR)
                keysStr += " fourth mouse ";
            if (mouseFlags & MOUSE_BUTTON_FIVE)
                keysStr += " fifth mouse ";
            if (mouseFlags & MOUSE_SCROLL_UP)
                keysStr += " scroll up ";
            if (mouseFlags & MOUSE_SCROLL_DOWN)
                keysStr += " scroll down ";
            if (mouseFlags & MOUSE_SCROLL_LEFT)
                keysStr += " scroll left ";
            if (mouseFlags & MOUSE_SCROLL_RIGHT)
                keysStr += " scroll right ";
            return keysStr;
        }

        void showKeysImGui(bool allowDelete = false) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !allowDelete);
            if (keyFlags.keyFlags.none() && mouseFlags == 0) {
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(223, 230, 233, 255));
                ImGui::SmallButton("No bindings");
                ImGui::PopStyleColor(1);
                ImGui::PopItemFlag();
                return;
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 234, 167, 255));
            for (int i = 340; i <= 348; i++) {
                if (keyFlags.keyFlags[i]) {
                    if (ImGui::SmallButton(keyNames[i].c_str()) && allowDelete)
                        keyFlags.keyFlags.reset(i);
                    if (ImGui::IsItemHovered() && allowDelete)
                        ImGui::SetTooltip("Remove");
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(162, 155, 254, 255));
            for (int i = 256; i <= 339; i++) {
                if (keyFlags.keyFlags[i]) {
                    if (ImGui::SmallButton(keyNames[i].c_str()) && allowDelete)
                        keyFlags.keyFlags.reset(i);
                    if (ImGui::IsItemHovered() && allowDelete)
                        ImGui::SetTooltip("Remove");
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(129, 236, 236, 255));
            for (int i = 0; i <= 255; i++) {
                if (keyFlags.keyFlags[i]) {
                    if (ImGui::SmallButton(keyNames[i].c_str()) && allowDelete)
                        keyFlags.keyFlags.reset(i);
                    if (ImGui::IsItemHovered() && allowDelete)
                        ImGui::SetTooltip("Remove");
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(250, 177, 160, 255));
            if (mouseFlags & MOUSE_BUTTON_LEFT) {
                if (ImGui::SmallButton("left mouse") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_BUTTON_RIGHT) {
                if (ImGui::SmallButton("right mouse") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_BUTTON_MIDDLE) {
                if (ImGui::SmallButton("middle mouse") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_BUTTON_FOUR) {
                if (ImGui::SmallButton("forth mouse") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_BUTTON_FIVE) {
                if (ImGui::SmallButton("fifth mouse") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_SCROLL_UP) {
                if (ImGui::SmallButton("scroll up") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_SCROLL_DOWN) {
                if (ImGui::SmallButton("scroll down") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_SCROLL_LEFT) {
                if (ImGui::SmallButton("scroll left") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            if (mouseFlags & MOUSE_SCROLL_RIGHT) {
                if (ImGui::SmallButton("scroll right") && allowDelete)
                    mouseFlags &= ~MOUSE_BUTTON_LEFT;
                if (ImGui::IsItemHovered() && allowDelete)
                    ImGui::SetTooltip("Remove");
                ImGui::SameLine();
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(99, 110, 114, 255));
            for (int i = 349; i <= 511; i++) {
                if (keyFlags.keyFlags[i]) {
                    ImGui::SmallButton(keyNames[i].c_str());
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

        static Binding now() {
            Binding hk;
            for (int i = 0; i < 512; i++) {
                if (ImGui::GetIO().KeysDown[i])
                    hk.keyFlags.addKey(i);
            }
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                hk.mouseFlags |= MOUSE_BUTTON_LEFT;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                hk.mouseFlags |= MOUSE_BUTTON_RIGHT;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                hk.mouseFlags |= MOUSE_BUTTON_MIDDLE;
            if (ImGui::IsMouseDown(3))
                hk.mouseFlags |= MOUSE_BUTTON_FOUR;
            if (ImGui::IsMouseDown(4))
                hk.mouseFlags |= MOUSE_BUTTON_FIVE;
            if (ImGui::GetIO().MouseWheel > FLT_MIN)
                hk.mouseFlags |= MOUSE_SCROLL_UP;
            if (ImGui::GetIO().MouseWheel < -FLT_MIN)
                hk.mouseFlags |= MOUSE_SCROLL_DOWN;
            if (ImGui::GetIO().MouseWheelH < -FLT_MIN)
                hk.mouseFlags |= MOUSE_SCROLL_LEFT;
            if (ImGui::GetIO().MouseWheelH > FLT_MIN)
                hk.mouseFlags |= MOUSE_SCROLL_RIGHT;
            return hk;
        }
    };

    class KeyBind {
    private:
        std::vector<std::vector<Binding>> bindings;
        Binding enrollBinding;
        bool isEnrolling = false;
        Binding *enrollingBinding;
        int enrollingAction;

    public:
        bool showBindingsWindow = false;
        bool keyBindUpdated = false;
        bool saveKeyBinds = false;

        KeyBind() {
            bindings.resize(actions.size());
        }

        std::vector<Binding> &getBinding(int action) {
            return bindings[action];
        }

        // Keys (including modifiers): STRICT MATCH    Mouse buttons: INCLUDE
        std::pair<bool, std::tuple<float, int, int>> isBindingActive(int action) {
            for (auto b : bindings[action]) {
                if (!(b.keyFlags.keyFlags.none() && b.mouseFlags == 0)
                    && (b.mouseFlags & gfn::keybind::Binding::now().mouseFlags) == b.mouseFlags
                    && gfn::keybind::Binding::now().keyFlags.keyFlags == b.keyFlags.keyFlags) {
                    float vel = FLT_MAX;
                    if (b.mouseFlags & MOUSE_SCROLL_UP)
                        vel = std::min(vel, ImGui::GetIO().MouseWheel);
                    if (b.mouseFlags & MOUSE_SCROLL_DOWN)
                        vel = std::min(vel, -ImGui::GetIO().MouseWheel);
                    if (b.mouseFlags & MOUSE_SCROLL_LEFT)
                        vel = std::min(vel, ImGui::GetIO().MouseWheelH);
                    if (b.mouseFlags & MOUSE_SCROLL_RIGHT)
                        vel = std::min(vel, -ImGui::GetIO().MouseWheelH);

                    if (vel == FLT_MAX) // no velocity available
                        return {true, {-1, b.repeatStartMs, b.repeatIntervalMs}};
                    if (vel > 0) // not active/reverse
                        return {true, {vel, b.repeatStartMs, b.repeatIntervalMs}};
                }
            }
            return {false, {-1, -1, -1}};
        }

        std::pair<bool, std::tuple<float, int, int>> isBindingActive(Binding &binding) {
            bool noVelActive = false;
            if (!(binding.keyFlags.keyFlags.none() && binding.mouseFlags == 0)
                && (binding.mouseFlags & gfn::keybind::Binding::now().mouseFlags) == binding.mouseFlags
                && gfn::keybind::Binding::now().keyFlags.keyFlags == binding.keyFlags.keyFlags) {
                float vel = FLT_MAX;
                if (binding.mouseFlags & MOUSE_SCROLL_UP)
                    vel = std::min(vel, ImGui::GetIO().MouseWheel);
                if (binding.mouseFlags & MOUSE_SCROLL_DOWN)
                    vel = std::min(vel, -ImGui::GetIO().MouseWheel);
                if (binding.mouseFlags & MOUSE_SCROLL_LEFT)
                    vel = std::min(vel, ImGui::GetIO().MouseWheelH);
                if (binding.mouseFlags & MOUSE_SCROLL_RIGHT)
                    vel = std::min(vel, -ImGui::GetIO().MouseWheelH);

                if (vel == FLT_MAX) // no velocity available
                    return {true, {-1, binding.repeatStartMs, binding.repeatIntervalMs}};
                if (vel <= 0) // not active/reverse
                    return {false, {-1, binding.repeatStartMs, binding.repeatIntervalMs}};
                return {true, {vel, binding.repeatStartMs, binding.repeatIntervalMs}};
            }
            return {false, {-1, binding.repeatStartMs, binding.repeatIntervalMs}};
        }

        void showEnrollPopup() {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(400.0f, 200.0f), std::max(360.0f, 170.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("Enroll Action Binding");
            ImGui::Text(("Action: " + actions[enrollingAction]).c_str());
            ImGui::BeginChildFrame(ImGui::GetID("enroll_area"),
                                   ImVec2(ImGui::GetContentRegionAvail().x,
                                          ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()
                                          - ImGui::GetStyle().ItemInnerSpacing.y * 2.0f),
                                   ImGuiWindowFlags_NoMove);

            enrollBinding.keyFlags.keyFlags |= Binding::now().keyFlags.keyFlags;
            auto mouse = Binding::now().mouseFlags;

            if (ImGui::IsWindowHovered()) {
                enrollBinding.mouseFlags |= mouse & 0b11111;

                if (ImGui::IsWindowHovered()) {
                    if (mouse & MOUSE_SCROLL_UP) {
                        enrollBinding.mouseFlags |= MOUSE_SCROLL_UP;
                        enrollBinding.mouseFlags &= ~MOUSE_SCROLL_DOWN;
                    }
                    if (mouse & MOUSE_SCROLL_DOWN) {
                        enrollBinding.mouseFlags |= MOUSE_SCROLL_DOWN;
                        enrollBinding.mouseFlags &= ~MOUSE_SCROLL_UP;
                    }
                    if (mouse & MOUSE_SCROLL_LEFT) {
                        enrollBinding.mouseFlags |= MOUSE_SCROLL_LEFT;
                        enrollBinding.mouseFlags &= ~MOUSE_SCROLL_RIGHT;
                    }
                    if (mouse & MOUSE_SCROLL_RIGHT) {
                        enrollBinding.mouseFlags |= MOUSE_SCROLL_RIGHT;
                        enrollBinding.mouseFlags &= ~MOUSE_SCROLL_LEFT;
                    }
                }
            }

            enrollBinding.showKeysImGui(true);

            ImGui::EndChildFrame();

            ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 3.0f) - ImGui::GetStyle().ItemInnerSpacing.x);
            if (ImGui::Button("Reset")) {
                enrollBinding.mouseFlags = MOUSE_BUTTON_NONE;
                enrollBinding.keyFlags.reset();
            }
            ImGui::SameLine();
            static bool repeat = false;
            repeat = enrollBinding.repeatStartMs >= 0;
            ImGui::Checkbox("Repeat", &repeat);
            enrollBinding.repeatStartMs = repeat ? 500 : -1;
            /*if (repeat) {
                // show repeat sliders
                ImGui::SameLine();
                ImGui::PushItemWidth(40.0f);
                ImGui::DragInt("Start repeat", &enrollBinding.repeatStartMs,
                               1.0f, 0, 10000, "%dms", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::PushItemWidth(40.0f);
                ImGui::DragInt("Repeat interval", &enrollBinding.repeatIntervalMs,
                               1.0f, 0, 10000, "%dms", ImGuiSliderFlags_AlwaysClamp);
            }*/

            ImGui::SameLine(ImGui::GetWindowWidth() - 124.0f);
            if (ImGui::Button("Cancel")) {
                isEnrolling = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - 65.0f);
            if (ImGui::Button("Done")) {
                if (!(enrollBinding.keyFlags.keyFlags.none() && enrollBinding.mouseFlags == 0))
                    *enrollingBinding = enrollBinding;
                isEnrolling = false;
                keyBindUpdated = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopItemWidth();

            ImGui::EndPopup();
        }

        void showKeybindSetup() {
            ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(1000.0f, 100.0f), std::max(400.0f, 100.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("Key Binds");

            ImGui::BeginChild("ActionList",
                              ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 28.0f), true,
                              0);

            bool first = true;
            for (int i = 0; i < actions.size(); i++) {
                if (actions[i].empty())
                    continue;
                if (first) first = false;
                else
                    ImGui::Separator();
                ImGui::Text(actions[i].c_str());
                ImGui::SameLine(675.0f);

                int bId = 0;
                for (auto it = bindings[i].begin(); it != bindings[i].end();) {
                    if (isBindingActive(*it).first) {
                        ImGui::SetCursorPosX(555.0f);
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 184, 148, 255));
                        ImGui::Text("active");
                        ImGui::PopStyleColor();
                        ImGui::SameLine(675.0f);
                    }

                    ImGui::SetCursorPosX(600.0f);
                    //ImGui::Text(bindings[i].getKeyStr().c_str());
                    it->showKeysImGui();

                    ImGui::SameLine(ImGui::GetWindowWidth() - 132.0f);
                    if (ImGui::SmallButton(("Edit##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        enrollBinding = *it;
                        enrollingBinding = &*it;
                        isEnrolling = true;
                        ImGui::OpenPopup("Enroll Action Binding");
                    }

                    ImGui::SameLine();
                    if (ImGui::SmallButton(("-##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        it = bindings[i].erase(it);
                        keyBindUpdated = true;
                    } else
                        it++;

                    /*ImGui::SameLine();
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, it != bindings[i].begin());
                    ImGui::PushStyleColor(ImGuiCol_Button, it != bindings[i].begin() ?
                                                           IM_COL32(178, 190, 195, 255) : IM_COL32(9, 132, 227, 255));
                    if (ImGui::SmallButton(("^##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        std::swap(*it, *(it - 1));
                        //it--;
                        keyBindUpdated = true;
                    }
                    ImGui::PopItemFlag();

                    ImGui::SameLine();
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, it != bindings[i].end() - 1);
                    ImGui::PushStyleColor(ImGuiCol_Button, it != bindings[i].end() - 1 ?
                                                           IM_COL32(178, 190, 195, 255) : IM_COL32(9, 132, 227, 255));
                    if (ImGui::SmallButton(("v##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        std::swap(*it, *(it + 1));
                        //it--;
                        keyBindUpdated = true;
                    }
                    ImGui::PopItemFlag();
                    ImGui::PopStyleColor(2);
                    */
                }
                if (bindings[i].empty()) {
                    ImGui::SetCursorPosX(600.0f);
                    Binding temp;
                    temp.showKeysImGui();
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 70.0f);
                if (ImGui::SmallButton(("Enroll##" + std::to_string(i)).c_str())) {
                    bindings[i].push_back(Binding());
                    enrollBinding.keyFlags.reset();
                    enrollBinding.mouseFlags = MOUSE_BUTTON_NONE;
                    enrollingBinding = &bindings[i][bindings[i].size() - 1];
                    enrollingAction = i;
                    isEnrolling = true;
                    ImGui::OpenPopup("Enroll Action Binding");
                }
            }
            if (isEnrolling)
                showEnrollPopup();

            ImGui::EndChild();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 140.0f);
            if (ImGui::Button("Cancel")) {
                showBindingsWindow = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine(ImGui::GetWindowWidth() - 80.0f);
            if (ImGui::Button("Save")) {
                saveKeyBinds = true;
                showBindingsWindow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        void serialize(nlohmann::json &j) {
            for (int i = 0; i < actions.size(); i++) {
                nlohmann::json hotkey = nlohmann::json::array();
                for (auto b : bindings[i]) {
                    nlohmann::json binding;
                    nlohmann::json keys = nlohmann::json::array();
                    for (int k = 0; k < 512; k++) {
                        if (b.keyFlags.keyFlags[k])
                            keys.push_back(keyNames[k]);
                    }
                    if (b.mouseFlags & MOUSE_BUTTON_LEFT)
                        keys.push_back("left_mouse");
                    if (b.mouseFlags & MOUSE_BUTTON_RIGHT)
                        keys.push_back("right_mouse");
                    if (b.mouseFlags & MOUSE_BUTTON_MIDDLE)
                        keys.push_back("middle_mouse");
                    if (b.mouseFlags & MOUSE_BUTTON_FOUR)
                        keys.push_back("forth_mouse");
                    if (b.mouseFlags & MOUSE_BUTTON_FIVE)
                        keys.push_back("fifth_mouse");
                    if (b.mouseFlags & MOUSE_SCROLL_UP)
                        keys.push_back("scroll_up");
                    if (b.mouseFlags & MOUSE_SCROLL_DOWN)
                        keys.push_back("scroll_down");
                    if (b.mouseFlags & MOUSE_SCROLL_LEFT)
                        keys.push_back("scroll_left");
                    if (b.mouseFlags & MOUSE_SCROLL_RIGHT)
                        keys.push_back("scroll_right");
                    binding["Keys"] = keys;
                    binding["Start repeat (ms)"] = b.repeatStartMs;
                    binding["Repeat rate interval (ms)"] = b.repeatIntervalMs;
                    hotkey.push_back(binding);
                }
                j[actions[i]] = hotkey;
            }
        }

        void deserialize(nlohmann::json &j) {
            bindings.clear();
            bindings.resize(actions.size());
            for (auto&[action, bs] : j.items()) {
                for (auto &b : bs) {
                    int actionId = -1;
                    for (int i = 0; i < actions.size(); i++) {
                        if (actions[i] == action) {
                            actionId = i;
                            break;
                        }
                    }
                    if (actionId == -1)
                        continue;

                    gfn::keybind::Binding bd;
                    for (auto &key : b["Keys"]) {
                        if (key == "left_mouse")
                            bd.mouseFlags |= MOUSE_BUTTON_LEFT;
                        if (key == "right_mouse")
                            bd.mouseFlags |= MOUSE_BUTTON_RIGHT;
                        if (key == "middle_mouse")
                            bd.mouseFlags |= MOUSE_BUTTON_MIDDLE;
                        if (key == "forth_mouse")
                            bd.mouseFlags |= MOUSE_BUTTON_FOUR;
                        if (key == "fifth_mouse")
                            bd.mouseFlags |= MOUSE_BUTTON_FIVE;
                        if (key == "scroll_up")
                            bd.mouseFlags |= MOUSE_SCROLL_UP;
                        if (key == "scroll_down")
                            bd.mouseFlags |= MOUSE_SCROLL_DOWN;
                        if (key == "scroll_left")
                            bd.mouseFlags |= MOUSE_SCROLL_LEFT;
                        if (key == "scroll_right")
                            bd.mouseFlags |= MOUSE_SCROLL_RIGHT;

                        if (bd.mouseFlags)
                            continue;
                        int keyCode = -1;
                        for (int i = 0; i < 512; i++) {
                            if (keyNames[i] == key) {
                                keyCode = i;
                                break;
                            }
                        }
                        if (keyCode != -1)
                            bd.keyFlags.addKey(keyCode);
                    }
                    bd.repeatStartMs = b["Start repeat (ms)"];
                    bd.repeatIntervalMs = b["Repeat rate interval (ms)"];

                    bindings[actionId].push_back(bd);
                }
            }
        }
    };
}
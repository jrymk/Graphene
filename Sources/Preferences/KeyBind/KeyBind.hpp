#pragma once

#include <imgui.h>
#include <set>
#include <bitset>
#include <json.hpp>

#include "KeyNames.hpp"
#include "Actions.hpp"

namespace gfn::keybind {
    struct KeySet {
        std::bitset<512> keyFlags;

        KeySet() = default;

        void reset() { keyFlags.reset(); }

        void addKey(int key) { keyFlags.set(key); }

        void removeKey(int key) { keyFlags.reset(key); }
    };

    struct Binding {
        int mouseButtonFlags = 0;
        KeySet keyFlags;

        Binding() {
            mouseButtonFlags = MOUSEBUTTON_NONE;
            keyFlags.reset();
        }

        Binding(int _mouseButtonFlags, KeySet _keyFlags) {
            mouseButtonFlags = _mouseButtonFlags;
            keyFlags = _keyFlags;
        }

        std::string getKeyStr() {
            std::string keysStr;
            for (int k = 0; k < 512; k++) {
                if (keyFlags.keyFlags[k])
                    keysStr += (" " + keyNames[k] + " ");
            }
            if (mouseButtonFlags & MOUSEBUTTON_LEFT)
                keysStr += " left_mouse ";
            if (mouseButtonFlags & MOUSEBUTTON_RIGHT)
                keysStr += " right_mouse ";
            if (mouseButtonFlags & MOUSEBUTTON_MIDDLE)
                keysStr += " middle_mouse ";
            if (mouseButtonFlags & MOUSEBUTTON_FOUR)
                keysStr += " fourth_mouse ";
            if (mouseButtonFlags & MOUSEBUTTON_FIVE)
                keysStr += " fifth_mouse ";
            return keysStr;
        }

        void showKeysImGui() {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            if (keyFlags.keyFlags.none() && mouseButtonFlags == 0) {
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(219, 219, 219, 255));
                ImGui::SmallButton("No bindings");
                ImGui::PopStyleColor(1);
                ImGui::PopItemFlag();
                return;
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 218, 150, 255));
            for (int i = 340; i <= 348; i++) {
                if (keyFlags.keyFlags[i]) {
                    ImGui::SmallButton(keyNames[i].c_str());
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(152, 255, 150, 255));
            for (int i = 256; i <= 339; i++) {
                if (keyFlags.keyFlags[i]) {
                    ImGui::SmallButton(keyNames[i].c_str());
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(150, 220, 255, 255));
            for (int i = 0; i <= 255; i++) {
                if (keyFlags.keyFlags[i]) {
                    ImGui::SmallButton(keyNames[i].c_str());
                    ImGui::SameLine();
                }
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 161, 161, 255));
            if (mouseButtonFlags & MOUSEBUTTON_LEFT) {
                ImGui::SmallButton("left_mouse");
                ImGui::SameLine();
            }
            if (mouseButtonFlags & MOUSEBUTTON_RIGHT) {
                ImGui::SmallButton("right_mouse");
                ImGui::SameLine();
            }
            if (mouseButtonFlags & MOUSEBUTTON_MIDDLE) {
                ImGui::SmallButton("middle_mouse");
                ImGui::SameLine();
            }
            if (mouseButtonFlags & MOUSEBUTTON_FOUR) {
                ImGui::SmallButton("forth_mouse");
                ImGui::SameLine();
            }
            if (mouseButtonFlags & MOUSEBUTTON_FIVE) {
                ImGui::SmallButton("fifth_mouse");
                ImGui::SameLine();
            }
            ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(179, 179, 179, 255));
            for (int i = 349; i <= 511; i++) {
                if (keyFlags.keyFlags[i]) {
                    ImGui::SmallButton(keyNames[i].c_str());
                    ImGui::SameLine();
                }
            }
            ImGui::Text("");
            ImGui::PopStyleColor(5);
            ImGui::PopItemFlag();
        }

        static Binding now() {
            Binding hk;
            for (int i = 0; i < 512; i++) {
                if (ImGui::GetIO().KeysDown[i])
                    hk.keyFlags.addKey(i);
            }
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                hk.mouseButtonFlags |= MOUSEBUTTON_LEFT;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                hk.mouseButtonFlags |= MOUSEBUTTON_RIGHT;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                hk.mouseButtonFlags |= MOUSEBUTTON_MIDDLE;
            if (ImGui::IsMouseDown(3))
                hk.mouseButtonFlags |= MOUSEBUTTON_FOUR;
            if (ImGui::IsMouseDown(4))
                hk.mouseButtonFlags |= MOUSEBUTTON_FIVE;
            return hk;
        }
    };

    class KeyBind {
    private:
        std::vector<std::vector<Binding>> bindings;
        Binding enrollBinding;
        int enrolling = -1;

    public:
        bool showBindingsWindow = false;
        bool keyBindUpdated = false;

        KeyBind() {
            bindings.resize(actions.size());
        }

        void bind(int action, Binding binding) {
            bindings[action].push_back(binding);
        }

        void clear(int action) {
            bindings[action].clear();
        }

        std::vector<Binding>& getBinding(int action) {
            return bindings[action];
        }

        // Keys (including modifiers): STRICT MATCH    Mouse buttons: INCLUDE
        bool isBindingActive(int action) {
            for (auto b : bindings[action]) {
                if (!(b.keyFlags.keyFlags.none() && b.mouseButtonFlags == 0)
                    && (b.mouseButtonFlags & gfn::keybind::Binding::now().mouseButtonFlags) == b.mouseButtonFlags
                    && gfn::keybind::Binding::now().keyFlags.keyFlags == b.keyFlags.keyFlags)
                    return true;
            }
            return false;
        }

        void showEnrollPopup(int action) {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(400.0f, 200.0f), std::max(360.0f, 170.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("Enroll Action Binding");
            ImGui::Text(("Action: " + actions[action]).c_str());
            ImGui::BeginChildFrame(ImGui::GetID("enroll_area"),
                                   ImVec2(ImGui::GetContentRegionAvail().x,
                                          ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()
                                          - ImGui::GetStyle().ItemInnerSpacing.y * 2.0f),
                                   ImGuiWindowFlags_NoMove);

            enrollBinding.keyFlags.keyFlags |= Binding::now().keyFlags.keyFlags;
            if (ImGui::IsWindowHovered())
                enrollBinding.mouseButtonFlags |= Binding::now().mouseButtonFlags;

            enrollBinding.showKeysImGui();

            ImGui::EndChildFrame();

            ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 3.0f) - ImGui::GetStyle().ItemInnerSpacing.x);
            if (ImGui::Button("Reset")) {
                enrollBinding.mouseButtonFlags = MOUSEBUTTON_NONE;
                enrollBinding.keyFlags.reset();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")) {
                enrolling = -1;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Done")) {
                bind(action, enrollBinding);
                enrolling = -1;
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

            ImGui::BeginChild("ActionList", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 30.0f), true, 0);
            for (int i = 0; i < actions.size(); i++) {
                if (actions[i].empty())
                    continue;

                ImGui::Separator();
                ImGui::Text(actions[i].c_str());
                ImGui::SameLine(675.0f);

                int bId = 0;
                for (auto it = bindings[i].begin(); it != bindings[i].end();) {
                    if (isBindingActive(i)) {
                        ImGui::SetCursorPosX(600.0f);
                        ImGui::Text("ACTIVE");
                    }
                    //ImGui::Text(bindings[i].getKeyStr().c_str());
                    ImGui::SetCursorPosX(675.0f);
                    it->showKeysImGui();

                    ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f);
                    if (ImGui::SmallButton(("-##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        it = bindings[i].erase(it);
                        keyBindUpdated = true;
                    }
                    else
                        it++;
                }

                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 70.0f);
                if (ImGui::SmallButton(("Enroll##" + std::to_string(i)).c_str())) {
                    enrollBinding.mouseButtonFlags = MOUSEBUTTON_NONE;
                    enrollBinding.keyFlags.reset();
                    enrolling = i;
                    ImGui::OpenPopup("Enroll Action Binding");
                }
            }
            if (enrolling >= 0)
                showEnrollPopup(enrolling);

            ImGui::EndChild();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 80.0f);
            if (ImGui::Button("Done")) {
                showBindingsWindow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        void serialize(nlohmann::json& j) {
            for (int i = 0; i < actions.size(); i++) {
                nlohmann::json hotkey = nlohmann::json::array();
                for (auto b : bindings[i]) {
                    nlohmann::json binding = nlohmann::json::array();
                    for (int k = 0; k < 512; k++) {
                        if (b.keyFlags.keyFlags[k])
                            binding.push_back(keyNames[k]);
                    }
                    if (b.mouseButtonFlags & MOUSEBUTTON_LEFT)
                        binding.push_back("left_mouse");
                    if (b.mouseButtonFlags & MOUSEBUTTON_RIGHT)
                        binding.push_back("right_mouse");
                    if (b.mouseButtonFlags & MOUSEBUTTON_MIDDLE)
                        binding.push_back("middle_mouse");
                    if (b.mouseButtonFlags & MOUSEBUTTON_FOUR)
                        binding.push_back("forth_mouse");
                    if (b.mouseButtonFlags & MOUSEBUTTON_FIVE)
                        binding.push_back("fifth_mouse");
                    hotkey.push_back(binding);
                }
                j[actions[i]] = hotkey;
            }
        }

        void deserialize(nlohmann::json& j) {
            bindings.clear();
            bindings.resize(actions.size());
            for (auto&[action, bs] : j.items()) {
                for (auto& b : bs) {
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
                    for (auto& key : b) {
                        if (key == "left_mouse")
                            bd.mouseButtonFlags |= MOUSEBUTTON_LEFT;
                        if (key == "right_mouse")
                            bd.mouseButtonFlags |= MOUSEBUTTON_RIGHT;
                        if (key == "middle_mouse")
                            bd.mouseButtonFlags |= MOUSEBUTTON_MIDDLE;
                        if (key == "forth_mouse")
                            bd.mouseButtonFlags |= MOUSEBUTTON_FOUR;
                        if (key == "fifth_mouse")
                            bd.mouseButtonFlags |= MOUSEBUTTON_FIVE;
                        if (bd.mouseButtonFlags)
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

                    bindings[actionId].push_back(bd);
                }
            }
        }
    };
}
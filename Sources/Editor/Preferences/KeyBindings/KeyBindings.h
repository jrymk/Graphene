#ifndef GRAPHENE_KEYBINDINGS_H
#define GRAPHENE_KEYBINDINGS_H

#include "Actions.h"
#include <imgui.h>
#include <string>
#include <bitset>
#include <vector>
#include <json.hpp>

namespace gfn {
    enum MouseNames {
        MOUSE_BUTTON_NONE = 0,
        MOUSE_BUTTON_LEFT = 1 << 0,
        MOUSE_BUTTON_RIGHT = 1 << 1,
        MOUSE_BUTTON_MIDDLE = 1 << 2,
        MOUSE_BUTTON_FOUR = 1 << 3,
        MOUSE_BUTTON_FIVE = 1 << 4,
        MOUSE_SCROLL_UP = 1 << 5,
        MOUSE_SCROLL_DOWN = 1 << 6,
        MOUSE_SCROLL_LEFT = 1 << 7,
        MOUSE_SCROLL_RIGHT = 1 << 8,
    };

    struct HotKey {
        static std::string keyNames[512];
        std::bitset<512> keyboard;
        int mouse = 0;

        int repeatStartMs = -1;
        int repeatIntervalMs = 100;

        HotKey();

        void showKeysImGui(bool allowDelete = false);

        static HotKey now();
    };

    struct Bindings {
    private:
        std::vector<std::vector<HotKey>> bindings;
        HotKey _enroll_temp_hk;
        HotKey* _enroll_target_hk = nullptr;
        int _enroll_target_action = -1;

    public:
        static std::vector<std::string> actionNames;

        bool showBindingsConfigWindow = false;
        bool bindingsUpdated = false;
        bool wantSaveBindings = false;

        Bindings();

        std::vector<HotKey>& getHotKeys(int action);

        /// @returns \<0>: active? \<1>: velocity \<2>: first matching hot key
        std::tuple<bool, float, HotKey> isBindingActive(int action);

        /// @returns <0>: active? <1>: velocity <2>: first matching hot key
        std::tuple<bool, float, HotKey> isBindingActive(HotKey& hotkey);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);

        void showEnrollPopup() {
            ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
            ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                    ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2(std::max(400.0f, 200.0f), std::max(360.0f, 170.0f)), ImGuiCond_Appearing);

            ImGui::BeginPopupModal("Enroll Action Binding");
            ImGui::Text(("Action: " + actionNames[_enroll_target_action]).c_str());
            ImGui::BeginChildFrame(ImGui::GetID("enroll_area"),
                                   ImVec2(ImGui::GetContentRegionAvail().x,
                                          ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()
                                          - ImGui::GetStyle().ItemInnerSpacing.y * 2.0f),
                                   ImGuiWindowFlags_NoMove);

            _enroll_temp_hk.keyboard |= HotKey::now().keyboard;
            auto mouse = HotKey::now().mouse;

            if (ImGui::IsWindowHovered()) {
                _enroll_temp_hk.mouse |= mouse & 0b11111;

                if (ImGui::IsWindowHovered()) {
                    if (mouse & MOUSE_SCROLL_UP) {
                        _enroll_temp_hk.mouse |= MOUSE_SCROLL_UP;
                        _enroll_temp_hk.mouse &= ~MOUSE_SCROLL_DOWN;
                    }
                    if (mouse & MOUSE_SCROLL_DOWN) {
                        _enroll_temp_hk.mouse |= MOUSE_SCROLL_DOWN;
                        _enroll_temp_hk.mouse &= ~MOUSE_SCROLL_UP;
                    }
                    if (mouse & MOUSE_SCROLL_LEFT) {
                        _enroll_temp_hk.mouse |= MOUSE_SCROLL_LEFT;
                        _enroll_temp_hk.mouse &= ~MOUSE_SCROLL_RIGHT;
                    }
                    if (mouse & MOUSE_SCROLL_RIGHT) {
                        _enroll_temp_hk.mouse |= MOUSE_SCROLL_RIGHT;
                        _enroll_temp_hk.mouse &= ~MOUSE_SCROLL_LEFT;
                    }
                }
            }

            _enroll_temp_hk.showKeysImGui(true);

            ImGui::EndChildFrame();

            ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 3.0f) - ImGui::GetStyle().ItemInnerSpacing.x);
            if (ImGui::Button("Reset")) {
                _enroll_temp_hk.mouse = MOUSE_BUTTON_NONE;
                _enroll_temp_hk.keyboard.reset();
            }
            ImGui::SameLine();
            static bool repeat = false;
            repeat = _enroll_temp_hk.repeatStartMs >= 0;
            ImGui::Checkbox("Repeat", &repeat);
            _enroll_temp_hk.repeatStartMs = repeat ? 500 : -1;
            /*if (repeat) {
                // show repeat sliders
                ImGui::SameLine();
                ImGui::PushItemWidth(40.0f);
                ImGui::DragInt("Start repeat", &_enroll_temp_hk.repeatStartMs,
                               1.0f, 0, 10000, "%dms", ImGuiSliderFlags_AlwaysClamp);
                ImGui::SameLine();
                ImGui::PushItemWidth(40.0f);
                ImGui::DragInt("Repeat interval", &_enroll_temp_hk.repeatIntervalMs,
                               1.0f, 0, 10000, "%dms", ImGuiSliderFlags_AlwaysClamp);
            }*/

            ImGui::SameLine(ImGui::GetWindowWidth() - 124.0f);
            if (ImGui::Button("Cancel")) {
                _enroll_target_action = -1;
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - 65.0f);
            if (ImGui::Button("Done")) {
                if (!(_enroll_temp_hk.keyboard.none() && _enroll_temp_hk.mouse == 0))
                    *_enroll_target_hk = _enroll_temp_hk;
                _enroll_target_action = -1;
                bindingsUpdated = true;
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
            for (int i = 0; i < actionNames.size(); i++) {
                if (actionNames[i].empty())
                    continue;
                if (first) first = false;
                else
                    ImGui::Separator();
                ImGui::Text(actionNames[i].c_str());
                ImGui::SameLine(675.0f);

                int bId = 0;
                for (auto it = bindings[i].begin(); it != bindings[i].end();) {
                    if (std::get<0>(isBindingActive(*it))) {
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
                        _enroll_temp_hk = *it;
                        _enroll_target_hk = &*it;
                        _enroll_target_action = i;
                        ImGui::OpenPopup("Enroll Action Binding");
                    }

                    ImGui::SameLine();
                    if (ImGui::SmallButton(("-##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        it = bindings[i].erase(it);
                        bindingsUpdated = true;
                    } else
                        it++;

                    /*ImGui::SameLine();
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, it != bindings[i].begin());
                    ImGui::PushStyleColor(ImGuiCol_Button, it != bindings[i].begin() ?
                                                           IM_COL32(178, 190, 195, 255) : IM_COL32(9, 132, 227, 255));
                    if (ImGui::SmallButton(("^##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        std::swap(*it, *(it - 1));
                        //it--;
                        bindingsUpdated = true;
                    }
                    ImGui::PopItemFlag();

                    ImGui::SameLine();
                    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, it != bindings[i].end() - 1);
                    ImGui::PushStyleColor(ImGuiCol_Button, it != bindings[i].end() - 1 ?
                                                           IM_COL32(178, 190, 195, 255) : IM_COL32(9, 132, 227, 255));
                    if (ImGui::SmallButton(("v##" + std::to_string(i) + "/" + std::to_string(bId++)).c_str())) {
                        std::swap(*it, *(it + 1));
                        //it--;
                        bindingsUpdated = true;
                    }
                    ImGui::PopItemFlag();
                    ImGui::PopStyleColor(2);
                    */
                }
                if (bindings[i].empty()) {
                    ImGui::SetCursorPosX(600.0f);
                    HotKey temp;
                    temp.showKeysImGui();
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 70.0f);
                if (ImGui::SmallButton(("Enroll##" + std::to_string(i)).c_str())) {
                    bindings[i].push_back(HotKey());
                    _enroll_temp_hk.keyboard.reset();
                    _enroll_temp_hk.mouse = MOUSE_BUTTON_NONE;
                    _enroll_target_hk = &bindings[i][bindings[i].size() - 1];
                    _enroll_target_action = i;
                    ImGui::OpenPopup("Enroll Action Binding");
                }
            }
            if (_enroll_target_action >= 0)
                showEnrollPopup();

            ImGui::EndChild();

            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 140.0f);
            if (ImGui::Button("Cancel")) {
                showBindingsConfigWindow = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine(ImGui::GetWindowWidth() - 80.0f);
            if (ImGui::Button("Save")) {
                wantSaveBindings = true;
                showBindingsConfigWindow = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    };
}

#endif
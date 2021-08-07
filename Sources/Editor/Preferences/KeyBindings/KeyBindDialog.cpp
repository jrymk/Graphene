#include "KeyBindings.h"

namespace gfn {
    void Bindings::showEnrollPopup() {
        ImGui::SetNextWindowSizeConstraints(ImVec2(200.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(std::max(400.0f, 200.0f), std::max(360.0f, 170.0f)), ImGuiCond_Appearing);

        ImGui::BeginPopupModal("Enroll Action Binding");
        ImGui::Text(("Action: " + actions[_enroll_target_action].first).c_str());
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
        _enroll_temp_hk.repeatStartMs = repeat ? 250 : -1;
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

    void Bindings::showKeybindSetup() {
        std::string condNames[5] = {"Hover on void", "Hover on unselected vertex", "Hover on unselected edge",
                                    "Hover on selected vertex", "Hover on selected edge"};

        ImGui::SetNextWindowSizeConstraints(ImVec2(300.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(std::max(800.0f, 100.0f), std::max(400.0f, 100.0f)), ImGuiCond_Appearing);

        ImGui::BeginPopupModal("Key Binds");

        ImGui::BeginChild("ActionList",
                          ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 28.0f), true,
                          0);

        bool first = true;
        for (int a = 0; a < actions.size(); a++) {
            if (actions[a].first.empty())
                continue;
            if (first) first = false;
            else
                ImGui::Separator();
            ImGui::Text(actions[a].first.c_str());

            for (int c = 0; c < 5; c++) {
                ImGui::SetCursorPosX(30.0f);
                ImGui::Text(condNames[c].c_str());
                ImGui::SameLine(300.0f);
                int bId = 0;
                for (auto it = bindings[a][c].begin(); it != bindings[a][c].end();) {
                    if (std::get<0>(isBindingActive(*it))) {
                        ImGui::SetCursorPosX(255.0f);
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 184, 148, 255));
                        ImGui::Text("active");
                        ImGui::PopStyleColor();
                        ImGui::SameLine(375.0f);
                    }

                    ImGui::SetCursorPosX(300.0f);
                    //ImGui::Text(bindings[i].getKeyStr().c_str());
                    it->showKeysImGui();

                    ImGui::SameLine(ImGui::GetWindowWidth() - 132.0f);
                    if (ImGui::SmallButton(("Edit##" + std::to_string(a) + "/" + std::to_string(c) + "/" + std::to_string(bId++)).c_str())) {
                        _enroll_temp_hk = *it;
                        _enroll_target_hk = &*it;
                        _enroll_target_action = a;
                        ImGui::OpenPopup("Enroll Action Binding");
                    }

                    ImGui::SameLine();
                    if (ImGui::SmallButton(("-##" + std::to_string(a) + "/" + std::to_string(c) + "/" + std::to_string(bId++)).c_str())) {
                        it = bindings[a][c].erase(it);
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
                    if (bindings[a][c].empty()) {
                        ImGui::SetCursorPosX(600.0f);
                        HotKey temp;
                        temp.showKeysImGui();
                    }
                }

                ImGui::SameLine(ImGui::GetWindowWidth() - 70.0f);
                if (ImGui::SmallButton(("Enroll##" + std::to_string(a) + "/" + std::to_string(c)).c_str())) {
                    bindings[a][c].push_back(HotKey());
                    _enroll_temp_hk.keyboard.reset();
                    _enroll_temp_hk.mouse = MOUSE_BUTTON_NONE;
                    _enroll_target_hk = &bindings[a][c][bindings[a][c].size() - 1];
                    _enroll_target_action = a;
                    ImGui::OpenPopup("Enroll Action Binding");
                }
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
}
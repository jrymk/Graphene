#pragma once

#include <imgui.h>
#include <set>
#include <bitset>

namespace gfn::keybind {
    // these are default key codes, can be modified with the Graphene.ini config file
    const std::pair<std::string, int> keyCodes[] = {
            {"space",           32},
            {"'",               39},
            {",",               44},
            {"-",               45},
            {".",               46},
            {"/",               47},
            {"0",               48},
            {"1",               49},
            {"2",               50},
            {"3",               51},
            {"4",               52},
            {"5",               53},
            {"6",               54},
            {"7",               55},
            {"8",               56},
            {"9",               57},
            {";",               59},
            {"=",               61},
            {"A",               65},
            {"B",               66},
            {"C",               67},
            {"D",               68},
            {"E",               69},
            {"F",               70},
            {"G",               71},
            {"H",               72},
            {"I",               73},
            {"J",               74},
            {"K",               75},
            {"L",               76},
            {"M",               77},
            {"N",               78},
            {"O",               79},
            {"P",               80},
            {"Q",               81},
            {"R",               82},
            {"S",               83},
            {"T",               84},
            {"U",               85},
            {"V",               86},
            {"W",               87},
            {"X",               88},
            {"Y",               89},
            {"Z",               90},
            {"[",               91},
            {"\\",              92},
            {"]",               93},
            {"`",               96},
            {"esc",             256},
            {"enter",           257},
            {"tab",             258},
            {"backspace",       259},
            {"insert",          260},
            {"delete",          261},
            {"right",           262},
            {"left",            263},
            {"down",            264},
            {"up",              265},
            {"page_up",         266},
            {"page_down",       267},
            {"home",            268},
            {"end",             269},
            {"caps_lock",       280},
            {"scroll_lock",     281},
            {"num_lock",        282},
            {"print_screen",    283},
            {"pause",           284},
            {"f1",              290},
            {"f2",              291},
            {"f3",              292},
            {"f4",              293},
            {"f5",              294},
            {"f6",              295},
            {"f7",              296},
            {"f8",              297},
            {"f9",              298},
            {"f10",             299},
            {"f11",             300},
            {"f12",             301},
            {"f13",             302},
            {"f14",             303},
            {"f15",             304},
            {"f16",             305},
            {"f17",             306},
            {"f18",             307},
            {"f19",             308},
            {"f20",             309},
            {"f21",             310},
            {"f22",             311},
            {"f23",             312},
            {"f24",             313},
            {"f25",             314},
            {"numpad_0",        320},
            {"numpad_1",        321},
            {"numpad_2",        322},
            {"numpad_3",        323},
            {"numpad_4",        324},
            {"numpad_5",        325},
            {"numpad_6",        326},
            {"numpad_7",        327},
            {"numpad_8",        328},
            {"numpad_9",        329},
            {"numpad_decimal",  330},
            {"numpad_divide",   331},
            {"numpad_multiply", 332},
            {"numpad_subtract", 333},
            {"numpad_add",      334},
            {"numpad_enter",    335},
            {"numpad_equal",    336},
            {"left_shift",      340},
            {"left_control",    341},
            {"left_alt",        342},
            {"left_super",      343},
            {"right_shift",     344},
            {"right_control",   345},
            {"right_alt",       346},
            {"right_super",     347},
            {"menu",            348},
    };

    struct KeyFlags {
        std::bitset<512> keyFlags;

        KeyFlags() = default;

        void reset() { keyFlags.reset(); }

        void addKey(int key) { keyFlags.set(key); }

        void removeKey(int key) { keyFlags.reset(key); }
    };

    KeyFlags nowKeyFlags() {
        KeyFlags kf;
        for (int i = 0; i < 512; i++) {
            if (ImGui::GetIO().KeysDown[i])
                kf.addKey(i);
        }
        return kf;
    }

//    enum MouseButtons {
//        MOUSEBUTTON_NONE = 0,
//        MOUSEBUTTON_LEFT = 1 << 0,
//        MOUSEBUTTON_RIGHT = 1 << 1,
//        MOUSEBUTTON_MIDDLE = 1 << 2,
//        MOUSEBUTTON_FOUR = 1 << 3,
//        MOUSEBUTTON_FIVE = 1 << 4,
//    };

    struct HotKey {
        int mouseButtonFlags;
        KeyFlags keyFlags;

        HotKey() {
            keyFlags.reset();
//            mouseButtonFlags = MOUSEBUTTON_NONE;
        }

        HotKey(int _mouseButtonFlags, KeyFlags _keyFlags) {
            keyFlags = _keyFlags;
            mouseButtonFlags = _mouseButtonFlags;
        }

        std::string getKeyStr() {
            std::string keysStr = "";
            for (auto& i : keyCodes) {
                if (keyFlags.keyFlags[i.second])
                    keysStr += (" " + i.first + " ");
            }
//            if (mouseButtonFlags & MOUSEBUTTON_LEFT)
//                keysStr += " left_mouse ";
//            if (mouseButtonFlags & MOUSEBUTTON_RIGHT)
//                keysStr += " right_mouse ";
//            if (mouseButtonFlags & MOUSEBUTTON_MIDDLE)
//                keysStr += " middle_mouse ";
            return keysStr;
        }
    };

    std::vector<std::string> actions = {
            "save document",
            "save as document",
            "open document",
    };

    enum Actions {
        ACTION_CAMERA_PAN = 0,
        ACTION_SELECTION_CLICK = 1,
        ACTION_SELECTION_LASSO = 2,
    };

    class KeyBind {
    private:
        std::vector<HotKey> bindings;

        HotKey enrollBinding;

        int enrolling = -1;

    public:
        bool showWindow = false;

        KeyBind() {
            bindings.resize(actions.size());
        }

        void bind(int action, HotKey binding) {
            bindings[action] = binding;
        }

        HotKey& getBinding(int action) {
            return bindings[action];
        }

        static bool isBindingActive(const KeyFlags& now, const HotKey& binding) {
//            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) != (binding.mouseButtonFlags & MOUSEBUTTON_LEFT))
//                return false;
//            if (ImGui::IsMouseDown(ImGuiMouseButton_Right) != (binding.mouseButtonFlags & MOUSEBUTTON_RIGHT))
//                return false;
//            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle) != (binding.mouseButtonFlags & MOUSEBUTTON_MIDDLE))
//                return false;
            if (now.keyFlags != binding.keyFlags.keyFlags)
                return false;
            return true;
        }

        void showEnrollPopup(int action) {
            ImGui::BeginPopupModal("Enroll Action HotKey");
            ImGui::Text((actions[action]).c_str());
            ImGui::BeginChildFrame(ImGui::GetID("enroll_area"),
                                   ImVec2(ImGui::GetContentRegionAvail().x,
                                          ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()
                                          - ImGui::GetStyle().ItemInnerSpacing.y * 2.0f),
                                   ImGuiWindowFlags_NoMove);
            if (ImGui::IsWindowHovered()) {
                enrollBinding.keyFlags.keyFlags |= nowKeyFlags().keyFlags;

//                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
//                    enrollBinding.mouseButtonFlags |= MOUSEBUTTON_LEFT;
//                if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
//                    enrollBinding.mouseButtonFlags |= MOUSEBUTTON_RIGHT;
//                if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
//                    enrollBinding.mouseButtonFlags |= MOUSEBUTTON_MIDDLE;
            }

            std::string disp = enrollBinding.getKeyStr();

            if (disp.empty()) {
                if (!ImGui::IsWindowHovered())
                    disp = "Hover here";
                else
                    disp = "Press keys and/or mouse buttons";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
            ImGui::Text(disp.c_str());
            auto bbSize = ImGui::GetItemRectSize();
            ImGui::PopStyleColor(1);
            ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvail().x - bbSize.x) / 2.0f,
                                       (ImGui::GetContentRegionAvail().y - bbSize.y) / 2.0f));
            ImGui::Text(disp.c_str());

            ImGui::EndChildFrame();

            ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 3.0f) - ImGui::GetStyle().ItemInnerSpacing.x);
            if (ImGui::Button("Reset")) {
//                enrollBinding.mouseButtonFlags = MOUSEBUTTON_NONE;
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
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopItemWidth();

            ImGui::End();
        }

        void showKeybindSetup() {
            ImGui::BeginPopupModal("Key Binds");
            for (int i = 0; i < actions.size(); i++) {
                ImGui::Text(actions[i].c_str());
                ImGui::SameLine(150.0f);
                ImGui::Text(bindings[i].getKeyStr().c_str());
                ImGui::SameLine(400.0f);
                if (ImGui::Button(("Enroll...##" + std::to_string(i)).c_str())) {
                    enrolling = i;
//                    enrollBinding.mouseButtonFlags = MOUSEBUTTON_NONE;
                    enrollBinding.keyFlags.reset();
                    ImGui::OpenPopup("Enroll Action HotKey");
                }
            }
            ImGui::SetCursorPos(
                    ImVec2(ImGui::GetWindowSize().x - ImGui::GetItemRectSize().x
                           - ImGui::GetStyle().ItemInnerSpacing.x,
                           ImGui::GetWindowSize().y - ImGui::GetItemRectSize().y -
                           ImGui::GetStyle().ItemInnerSpacing.y));
            if (ImGui::Button("Close")) {
                showWindow = false;
                ImGui::CloseCurrentPopup();
            }
            if (enrolling >= 0)
                showEnrollPopup(enrolling);
            ImGui::End();
        }
    };
}
#pragma once

#include <imgui.h>
#include <set>
#include <bitset>

namespace gfn::keybind {
    // these are default key codes, can be modified with the Graphene.ini config file
    const std::pair<std::string, int> keyCodes[] = {
            {"space",           32},
            {"apostrophe",      39},
            {"comma",           44},
            {"minus",           45},
            {"period",          46},
            {"slash",           47},
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
            {"semicolon",       59},
            {"equal",           61},
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
            {"left_bracket",    91},
            {"backslash",       92},
            {"right_bracket",   93},
            {"grave",           96},
            {"escape",          256},
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

        KeyFlags() { keyFlags.reset(); }

        void reset() { keyFlags.reset(); }

        void addKey(int key) { keyFlags.set(key); }

        void removeKey(int key) { keyFlags.reset(key); }

        void now() {
            for (int i = 0; i < 512; i++)
                keyFlags.set(i, ImGui::GetIO().KeysDown[i]);
        }
    };

    struct Binding {
        enum MouseButtons {
            MOUSEBUTTON_NONE = 0,
            MOUSEBUTTON_LEFT = 1 << 0,
            MOUSEBUTTON_MIDDLE = 1 << 1,
            MOUSEBUTTON_RIGHT = 1 << 2,
        };

        int mouseButtonFlags;
        KeyFlags keyFlags;

        Binding() {
            mouseButtonFlags = MOUSEBUTTON_NONE;
        }
    };

    enum Actions {
        ACTION_CAMERA_PAN = 0,

    };

    class KeyBind {
    private:
        std::vector<Binding> bindings;

        Binding enrollBinding;

    public:
        KeyBind() {
            bindings.resize(1);
            Binding binding;
            binding.mouseButtonFlags |= Binding::MOUSEBUTTON_LEFT;
            bindings[ACTION_CAMERA_PAN] = binding;
        }

        void bind(Actions action, Binding binding) {
            bindings[action] = binding;
        }

        Binding& getBinding(Actions action) {
            return bindings[action];
        }

        static bool isBindingActive(const Binding& binding) {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left) != (binding.mouseButtonFlags & Binding::MOUSEBUTTON_LEFT))
                return false;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Right) != (binding.mouseButtonFlags & Binding::MOUSEBUTTON_RIGHT))
                return false;
            if (ImGui::IsMouseDown(ImGuiMouseButton_Middle) != (binding.mouseButtonFlags & Binding::MOUSEBUTTON_MIDDLE))
                return false;
            KeyFlags now;
            now.now();
            if (now.keyFlags != binding.keyFlags.keyFlags)
                return false;
            return true;
        }

        void showKeybindSetup() {
            ImGui::Begin("Keybinds");
            
            ImGui::End();
        }

        void showEnrollPopup(Actions action) {
            ImGui::Begin("Enroll Action Binding");
            ImGui::BeginChildFrame(ImGui::GetID("enroll_area"),
                                   ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing()),
                                   ImGuiWindowFlags_NoMove);
            if (ImGui::IsWindowHovered()) {
                KeyFlags now;
                now.now();
                enrollBinding.keyFlags.keyFlags |= now.keyFlags;

                if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
                    enrollBinding.mouseButtonFlags |= Binding::MOUSEBUTTON_LEFT;
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                    enrollBinding.mouseButtonFlags |= Binding::MOUSEBUTTON_RIGHT;
                if (ImGui::IsMouseDown(ImGuiMouseButton_Middle))
                    enrollBinding.mouseButtonFlags |= Binding::MOUSEBUTTON_MIDDLE;
            }
            std::string keysStr = "";
            for (auto& i : keyCodes) {
                if (enrollBinding.keyFlags.keyFlags[i.second])
                    keysStr += ("[" + i.first + "] ");
            }
            if (enrollBinding.mouseButtonFlags & Binding::MOUSEBUTTON_LEFT)
                keysStr += "[left_mouse_button] ";
            if (enrollBinding.mouseButtonFlags & Binding::MOUSEBUTTON_RIGHT)
                keysStr += "[right_mouse_button] ";
            if (enrollBinding.mouseButtonFlags & Binding::MOUSEBUTTON_MIDDLE)
                keysStr += "[middle_mouse_button] ";

            if (keysStr.empty()) {
                if (!ImGui::IsWindowHovered())
                    keysStr = "Hover here";
                else
                    keysStr = "Press keys and/or mouse buttons";
            }
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 0));
            ImGui::Text(keysStr.c_str());
            auto bbSize = ImGui::GetItemRectSize();
            ImGui::PopStyleColor(1);
            ImGui::SetCursorPos(ImVec2((ImGui::GetContentRegionAvail().x - bbSize.x) / 2.0f,
                                       (ImGui::GetContentRegionAvail().y - bbSize.y) / 2.0f));
            ImGui::Text(keysStr.c_str());

            ImGui::EndChildFrame();

            if (ImGui::Button("Reset", ImVec2(ImGui::GetContentRegionAvail().x / 2.0f, ImGui::GetContentRegionAvail().y))) {
                enrollBinding.mouseButtonFlags = Binding::MOUSEBUTTON_NONE;
                enrollBinding.keyFlags.reset();
            }
            ImGui::SameLine();
            if (ImGui::Button("Done", ImGui::GetContentRegionAvail())) {
                bind(action, enrollBinding);
            }
            ImGui::End();
        }
    };
}
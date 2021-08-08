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

    enum Conditions {
        COND_VOID = 0,
        COND_UNSEL_VERTEX = 1,
        COND_UNSEL_EDGE = 2,
        COND_SEL_VERTEX = 3,
        COND_SEL_EDGE = 4,
    };

    struct HotKey {
        static std::string keyNames[512];
        std::bitset<512> keyboard;
        int mouse = 0;

        int repeatStartMs = -1;
        int repeatIntervalMs = 75;

        HotKey();

        void showKeysImGui(bool allowDelete = false);

        static HotKey now();
    };

    struct Bindings {
    private:
        std::vector<std::vector<std::vector<HotKey>>> bindings;
        HotKey _enroll_temp_hk;
        HotKey* _enroll_target_hk = nullptr;
        int _enroll_target_action = -1;

    public:
        static int nonCondActionStartsAt;
        static std::vector<std::pair<std::string, int>> actions;

        bool showBindingsConfigWindow = false;
        bool bindingsUpdated = false;
        bool wantSaveBindings = false;

        Bindings();

        std::vector<HotKey>& getHotKeys(int action, int condition);

        /// @returns \<0>: active? \<1>: velocity \<2>: first matching hot key
        std::tuple<bool, float, HotKey> isBindingActive(int action, int condition);

        /// @returns <0>: active? <1>: velocity <2>: first matching hot key
        std::tuple<bool, float, HotKey> isBindingActive(HotKey& hotkey);

        void serialize(nlohmann::json& j);

        void deserialize(nlohmann::json& j);

        void showEnrollPopup();

        void showKeybindSetup();
    };
}

#endif
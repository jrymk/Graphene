#pragma once

#include <Preferences/KeyBind.hpp>
#include <vector>

namespace gfn::editor {
    gfn::keybind::KeyBind keybind;

    std::vector<bool> hotKeyPress;

    std::vector<bool> hotKeyHold;

    std::vector<bool> hotKeyRelease;

    void initHotKey() {
        hotKeyPress.resize(gfn::keybind::actions.size());
        hotKeyHold.resize(gfn::keybind::actions.size());
        hotKeyRelease.resize(gfn::keybind::actions.size());
    }

    void updateHotKey() {
        auto now = gfn::keybind::nowKeyFlags();
        for (int i = 0; i < gfn::keybind::actions.size(); i++) {
            auto binding = keybind.getBinding(i);
            bool prev = hotKeyHold[i];
            hotKeyPress[i] = false;
            hotKeyRelease[i] = false;
            if (binding.keyFlags.keyFlags == now.keyFlags) {
                if (!prev)
                    hotKeyPress[i] = true;
                hotKeyHold[i] = true;
            } else {
                hotKeyHold[i] = false;
                if (prev)
                    hotKeyRelease[i] = true;
            }
        }
    }
}
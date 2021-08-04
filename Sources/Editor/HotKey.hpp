#pragma once

#include <Preferences/KeyBind/KeyBind.hpp>
#include <System/Timer/Timer.hpp>
#include <vector>

namespace gfn::editor {
    gfn::keybind::KeyBind keyBind;

    int doubleClickThresholdMs = 200;

    static std::vector<bool> _hotKeyPress;
    static std::vector<bool> _hotKeyDown;
    static std::vector<bool> _hotKeyRelease;
    static std::vector<bool> _hotKeyDoubleClick;

    static std::vector<gfn::timer::TimePoint> _pressTp;
    static std::vector<gfn::timer::TimePoint> _releaseTp;

    void initHotKey() {
        _hotKeyPress.resize(gfn::keybind::actions.size());
        _hotKeyDown.resize(gfn::keybind::actions.size());
        _hotKeyRelease.resize(gfn::keybind::actions.size());
        _hotKeyDoubleClick.resize(gfn::keybind::actions.size());
        _pressTp.resize(gfn::keybind::actions.size());
        _releaseTp.resize(gfn::keybind::actions.size());
    }

    void updateHotKey() {
        auto now = gfn::keybind::Binding::now();
        for (int i = 0; i < gfn::keybind::actions.size(); i++) {
            bool prev = _hotKeyDown[i];
            // reset pulse states
            _hotKeyPress[i] = false;
            _hotKeyRelease[i] = false;
            _hotKeyDoubleClick[i] = false;

            if (keyBind.isBindingActive(i)) {
                if (!prev) {
                    _hotKeyPress[i] = true;
                    _pressTp[i].restart();
                    // detect double click
                    if (gfn::timer::TimePoint::getMilliseconds(_releaseTp[i]) < doubleClickThresholdMs)
                        _hotKeyDoubleClick[i] = true;
                }
                _hotKeyDown[i] = true;
            } else {
                _hotKeyDown[i] = false;
                if (prev) {
                    _hotKeyRelease[i] = true;
                    _releaseTp[i].restart();
                }
            }
        }
    }

    bool hkPress(int action) { return _hotKeyPress[action]; }

    bool hkDown(int action) { return _hotKeyDown[action]; }

    bool hkRelease(int action) { return _hotKeyRelease[action]; }

    bool hkDoubleClick(int action) { return _hotKeyDoubleClick[action]; }

    unsigned long long hkTimeSincePressMs(int action) { return gfn::timer::TimePoint::getMilliseconds(_pressTp[action]); }

    unsigned long long hkTimeSinceReleaseMs(int action) { return gfn::timer::TimePoint::getMilliseconds(_releaseTp[action]); }
}
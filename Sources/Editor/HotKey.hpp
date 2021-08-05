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
    static std::vector<float> _hotKeyVelocity;

    static std::vector<gfn::timer::TimePoint> _pressTp;
    static std::vector<gfn::timer::TimePoint> _releaseTp;
    static std::vector<int> _repeatCnt;

    void initHotKey() {
        _hotKeyPress.resize(gfn::keybind::actions.size());
        _hotKeyDown.resize(gfn::keybind::actions.size());
        _hotKeyRelease.resize(gfn::keybind::actions.size());
        _hotKeyDoubleClick.resize(gfn::keybind::actions.size());
        _hotKeyVelocity.resize(gfn::keybind::actions.size());
        _pressTp.resize(gfn::keybind::actions.size());
        _releaseTp.resize(gfn::keybind::actions.size());
        _repeatCnt.resize(gfn::keybind::actions.size());
    }

    void updateHotKey() {
        for (int i = 0; i < gfn::keybind::actions.size(); i++) {
            bool prev = _hotKeyDown[i];
            // reset pulse states
            _hotKeyPress[i] = false;
            _hotKeyRelease[i] = false;
            _hotKeyDoubleClick[i] = false;

            auto active = keyBind.isBindingActive(i);
            if (active.first) {
                if (!prev) {
                    _hotKeyPress[i] = true;
                    _pressTp[i].restart();
                    _repeatCnt[i] = 0;
                    // detect double click
                    if (gfn::timer::TimePoint::getMilliseconds(_releaseTp[i]) < doubleClickThresholdMs)
                        _hotKeyDoubleClick[i] = true;
                }
                _hotKeyDown[i] = true;
                _hotKeyVelocity[i] = std::get<0>(active.second);

                if (std::get<1>(active.second) >= 0) { // 0: velocity, 1: start repeat 2: repeat rate
                    if (gfn::timer::TimePoint::getMilliseconds(_pressTp[i]) >= std::get<1>(active.second) + std::get<2>(active.second) * _repeatCnt[i]) {
                        _hotKeyPress[i] = true;
                        _repeatCnt[i]++;
                    }
                }
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

    bool hkHasVelocity(int action) { return _hotKeyVelocity[action] >= 0; }

    float hkVelocity(int action) { return _hotKeyVelocity[action]; }

    unsigned long long hkTimeSincePressMs(int action) { return gfn::timer::TimePoint::getMilliseconds(_pressTp[action]); }

    unsigned long long hkTimeSinceReleaseMs(int action) { return gfn::timer::TimePoint::getMilliseconds(_releaseTp[action]); }
}
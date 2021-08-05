#pragma once

#include <Preferences/KeyBinding/KeyBinding.h>
#include <System/Timer.hpp>
#include <vector>
#include <minitrace.h>

namespace gfn::editor {
    gfn::Bindings keyBind;

    int doubleClickThresholdMs = 200;

    static std::vector<bool> _hotKeyPress;
    static std::vector<bool> _hotKeyDown;
    static std::vector<bool> _hotKeyRelease;
    static std::vector<bool> _hotKeyDoubleClick;
    static std::vector<float> _hotKeyVelocity;

    static std::vector<gfn::TimePoint> _pressTp;
    static std::vector<gfn::TimePoint> _releaseTp;
    static std::vector<int> _repeatCnt;

    void initHotKey() {
        _hotKeyPress.resize(gfn::Bindings::actionNames.size());
        _hotKeyDown.resize(gfn::Bindings::actionNames.size());
        _hotKeyRelease.resize(gfn::Bindings::actionNames.size());
        _hotKeyDoubleClick.resize(gfn::Bindings::actionNames.size());
        _hotKeyVelocity.resize(gfn::Bindings::actionNames.size());
        _pressTp.resize(gfn::Bindings::actionNames.size());
        _releaseTp.resize(gfn::Bindings::actionNames.size());
        _repeatCnt.resize(gfn::Bindings::actionNames.size());
    }

    void updateHotKey() {
        MTR_SCOPE("main update", "update hotkey");

        for (int i = 0; i < gfn::Bindings::actionNames.size(); i++) {
            bool prev = _hotKeyDown[i];
            // reset pulse states
            _hotKeyPress[i] = false;
            _hotKeyRelease[i] = false;
            _hotKeyDoubleClick[i] = false;

            auto active = keyBind.isBindingActive(i);

            if (std::get<0>(active)) {
                if (!prev) {
                    _hotKeyPress[i] = true;
                    _pressTp[i].restart();
                    _repeatCnt[i] = 0;
                    // detect double click
                    if (gfn::TimePoint::getMilliseconds(_releaseTp[i]) < doubleClickThresholdMs)
                        _hotKeyDoubleClick[i] = true;
                }
                _hotKeyDown[i] = true;
                _hotKeyVelocity[i] = std::get<1>(active);

                if (std::get<2>(active).repeatStartMs >= 0) {
                    if (gfn::TimePoint::getMilliseconds(_pressTp[i]) >= std::get<2>(active).repeatStartMs + std::get<2>(active).repeatIntervalMs * _repeatCnt[i]) {
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

    unsigned long long hkTimeSincePressMs(int action) { return gfn::TimePoint::getMilliseconds(_pressTp[action]); }

    unsigned long long hkTimeSinceReleaseMs(int action) { return gfn::TimePoint::getMilliseconds(_releaseTp[action]); }
}
#include "HotKeyHandler.h"
#include <iostream>

namespace gfn {
    HKHandler::HKHandler(gfn::Preferences* prefs) :
            prefs(prefs),
            _hotKeyPress(gfn::Bindings::actions.size(), std::vector<bool>(5)),
            _hotKeyDown(gfn::Bindings::actions.size(), std::vector<bool>(5)),
            _hotKeyRelease(gfn::Bindings::actions.size(), std::vector<bool>(5)),
            _hotKeyDoubleClick(gfn::Bindings::actions.size(), std::vector<bool>(5)),
            _hotKeyVelocity(gfn::Bindings::actions.size(), std::vector<float>(5)),
            _pressTp(gfn::Bindings::actions.size(), std::vector<gfn::TimePoint>(5)),
            _releaseTp(gfn::Bindings::actions.size(), std::vector<gfn::TimePoint>(5)),
            _repeatCnt(gfn::Bindings::actions.size(), std::vector<int>(5)) {
    }

    void HKHandler::updateHotKeyState() {
        for (int a = 0; a < gfn::Bindings::actions.size(); a++) {
            for (int c = 0; c < 5; c++) {
                bool prev = _hotKeyDown[a][c];
                // reset pulse states
                _hotKeyPress[a][c] = false;
                _hotKeyRelease[a][c] = false;
                _hotKeyDoubleClick[a][c] = false;

                auto active = prefs->bindings.isBindingActive(a, c);

                if (std::get<0>(active)) { // really active
                    if (!prev) {
                        _hotKeyPress[a][c] = true;
                        _pressTp[a][c].restart();
                        _repeatCnt[a][c] = 0;
                        // detect double click
                        if (gfn::TimePoint::getMilliseconds(_releaseTp[a][c]) < doubleClickThresholdMs)
                            _hotKeyDoubleClick[a][c] = true;
                    }
                    _hotKeyDown[a][c] = true;
                    _hotKeyVelocity[a][c] = std::get<1>(active);

                    if (std::get<2>(active).repeatStartMs >= 0) {
                        if (gfn::TimePoint::getMilliseconds(_pressTp[a][c]) >=
                            std::get<2>(active).repeatStartMs + std::get<2>(active).repeatIntervalMs * _repeatCnt[a][c]) {
                            _hotKeyPress[a][c] = true;
                            _repeatCnt[a][c]++;
                        }
                    }
                } else {
                    _hotKeyDown[a][c] = false;
                    if (prev) {
                        _hotKeyRelease[a][c] = true;
                        _releaseTp[a][c].restart();
                    }
                }
            }
        }
    }

    bool HKHandler::press(int actionId, int condition) {
        if (condition < 0) {
            for (int i = 0; i < 5; i++)
                if (_hotKeyPress[actionId][i])
                    return true;
            return false;
        }
        return _hotKeyPress[actionId][condition];
    }

    bool HKHandler::down(int actionId, int condition) {
        if (condition < 0) {
            for (int i = 0; i < 5; i++)
                if (_hotKeyDown[actionId][i])
                    return true;
            return false;
        }
        return _hotKeyDown[actionId][condition];
    }

    bool HKHandler::release(int actionId, int condition) {
        if (condition < 0) {
            for (int i = 0; i < 5; i++)
                if (_hotKeyRelease[actionId][i])
                    return true;
            return false;
        }
        return _hotKeyRelease[actionId][condition];
    }

    bool HKHandler::doubleClick(int actionId, int condition) {
        if (condition < 0) {
            for (int i = 0; i < 5; i++)
                if (_hotKeyDoubleClick[actionId][i])
                    return true;
            return false;
        }
        return _hotKeyDoubleClick[actionId][condition];
    }

    bool HKHandler::hasVelocity(int actionId, int condition) { return _hotKeyVelocity[actionId][condition] >= 0; }

    float HKHandler::velocity(int actionId, int condition) { return _hotKeyVelocity[actionId][condition]; }

    unsigned long long HKHandler::timeSincePressMs(int actionId, int condition) { return gfn::TimePoint::getMilliseconds(_pressTp[actionId][condition]); }

    unsigned long long HKHandler::timeSinceReleaseMs(int actionId, int condition) { return gfn::TimePoint::getMilliseconds(_releaseTp[actionId][condition]); }
}
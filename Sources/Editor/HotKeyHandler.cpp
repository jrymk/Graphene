#include "HotKeyHandler.h"
#include <minitrace.h>

namespace gfn {
    HKHandler::HKHandler(gfn::Preferences* prefs) : prefs(prefs) {
        _hotKeyPress.resize(gfn::Bindings::actionNames.size());
        _hotKeyDown.resize(gfn::Bindings::actionNames.size());
        _hotKeyRelease.resize(gfn::Bindings::actionNames.size());
        _hotKeyDoubleClick.resize(gfn::Bindings::actionNames.size());
        _hotKeyVelocity.resize(gfn::Bindings::actionNames.size());
        _pressTp.resize(gfn::Bindings::actionNames.size());
        _releaseTp.resize(gfn::Bindings::actionNames.size());
        _repeatCnt.resize(gfn::Bindings::actionNames.size());
    }

    void HKHandler::update() {
        MTR_SCOPE("main update", "update hotkey");

        for (int i = 0; i < gfn::Bindings::actionNames.size(); i++) {
            bool prev = _hotKeyDown[i];
            // reset pulse states
            _hotKeyPress[i] = false;
            _hotKeyRelease[i] = false;
            _hotKeyDoubleClick[i] = false;

            auto active = prefs->bindings.isBindingActive(i);

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
                    if (gfn::TimePoint::getMilliseconds(_pressTp[i]) >=
                        std::get<2>(active).repeatStartMs + std::get<2>(active).repeatIntervalMs * _repeatCnt[i]) {
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

    bool HKHandler::press(int actionId) { return _hotKeyPress[actionId]; }

    bool HKHandler::down(int actionId) { return _hotKeyDown[actionId]; }

    bool HKHandler::release(int actionId) { return _hotKeyRelease[actionId]; }

    bool HKHandler::doubleClick(int actionId) { return _hotKeyDoubleClick[actionId]; }

    bool HKHandler::hasVelocity(int actionId) { return _hotKeyVelocity[actionId] >= 0; }

    float HKHandler::velocity(int actionId) { return _hotKeyVelocity[actionId]; }

    unsigned long long HKHandler::timeSincePressMs(int actionId) { return gfn::TimePoint::getMilliseconds(_pressTp[actionId]); }

    unsigned long long HKHandler::timeSinceReleaseMs(int actionId) { return gfn::TimePoint::getMilliseconds(_releaseTp[actionId]); }
}
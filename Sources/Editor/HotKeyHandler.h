#ifndef HOTKEYHANDLER_H
#define HOTKEYHANDLER_H

#include <vector>
#include <Editor/Preferences/Preferences.h>
#include <Core/Objects/Timer.h>

namespace gfn {
    // the selection module, or whatever using the hotkey handler should handle the different state
    // themselves. this just checks for 5 mouse conditions defined in bindings.
    class HKHandler {
    private:
        gfn::Preferences* prefs;

        std::vector<std::vector<bool>> _hotKeyPress;   // hot key keeps getting presses depite cond not matching anymore
        std::vector<std::vector<bool>> _hotKeyDown;    // hot key stays down despite cond not matching anymore
        std::vector<std::vector<bool>> _hotKeyRelease; // release raw is not a thing
        std::vector<std::vector<bool>> _hotKeyDoubleClick;
        std::vector<std::vector<float>> _hotKeyVelocity;
        std::vector<std::vector<gfn::TimePoint>> _pressTp;
        std::vector<std::vector<gfn::TimePoint>> _releaseTp;
        std::vector<std::vector<int>> _repeatCnt;

    public:
        explicit HKHandler(gfn::Preferences* prefs);

        int doubleClickThresholdMs = 200;

        void updateHotKeyState();

        bool press(int actionId, int condition = -1);

        bool down(int actionId, int condition = -1);

        bool release(int actionId, int condition = -1);

        bool doubleClick(int actionId, int condition = -1);

        bool hasVelocity(int actionId, int condition);

        float velocity(int actionId, int condition);

        unsigned long long timeSincePressMs(int actionId, int condition);

        unsigned long long timeSinceReleaseMs(int actionId, int condition);
    };
}

#endif
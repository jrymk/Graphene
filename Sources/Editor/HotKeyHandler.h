#ifndef HOTKEYHANDLER_H
#define HOTKEYHANDLER_H

#include <vector>
#include <Editor/Preferences/Preferences.h>
#include <Core/Objects/Timer.h>

namespace gfn {
    class HKHandler {
    private:
        gfn::Preferences* prefs;

        std::vector<bool> _hotKeyPress;
        std::vector<bool> _hotKeyDown;
        std::vector<bool> _hotKeyRelease;
        std::vector<bool> _hotKeyDoubleClick;
        std::vector<float> _hotKeyVelocity;
        std::vector<gfn::TimePoint> _pressTp;
        std::vector<gfn::TimePoint> _releaseTp;
        std::vector<int> _repeatCnt;

    public:
        explicit HKHandler(gfn::Preferences* prefs);

        int doubleClickThresholdMs = 200;

        void update();

        bool press(int actionId);

        bool down(int actionId);

        bool release(int actionId);

        bool doubleClick(int actionId);

        bool hasVelocity(int actionId);

        float velocity(int actionId);

        unsigned long long timeSincePressMs(int actionId);

        unsigned long long timeSinceReleaseMs(int actionId);
    };
}

#endif
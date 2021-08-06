#include "Core.h"

namespace gfn {
    void Core::startBackground() {
        bgState = 1; // set to RUNNING
        std::thread bgThread(&Core::bgFunction, this, std::ref(bgState));
        bgThread.detach();
    }

    void Core::terminateBackground(bool doNotWait) {
        bgState = 0;
        if (doNotWait)
            return;
        while (bgState != 2)
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        bgState = 0;
    }

    bool Core::checkTermination() { return (bgState == 2); }

    void Core::bgFunction(int& state) {
        while (state == 1) // while RUNNING
            update();
        state = 2; // TERMINATED
    }
}
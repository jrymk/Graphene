#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <chrono>

#include "utils/ExceptionHandler.hpp"
#include "graphene/Include.h"
#include "gui/Include.h"

void updateGraphLoop(Graphene::Core &core, bool &update, bool &end) {
    while (!end) {
        if (update)
            core.updatePos();
    }
}

int main() {

    Gui::init();

    Graphene::Core core;

    bool updateGraphAsync = true;
    // Multi-threading is temporarily disabled because it leads to crash
//    bool endBackgroundUpdateLoop = false;
//    std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(updateGraphAsync), std::ref(endBackgroundUpdateLoop));
//    backgroundUpdateThread.detach();

    while (!Gui::shouldClose()) {
        auto prevTime = std::chrono::high_resolution_clock::now();

        Gui::initFrame();
        Gui::Toolbar::show(&core);
        Gui::Input::show(&core);
        Gui::GraphView::show(&core);
        Gui::DebugWindow::show(&core);
        Gui::LogWindow::show();
        Gui::render();

        updateGraphAsync = Gui::Toolbar::updateGraph;
        if(updateGraphAsync) core.updatePos();

        core.updateRateCounter.updateWithoutCount();
    }

//    endBackgroundUpdateLoop = true;

    Gui::cleanup();
}
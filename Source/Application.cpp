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

//#include "ui/imgui_handler.hpp"

void updateGraphLoop(Graphene::Core &core, bool &update, bool &end) {
    while (!end) {
        if (update)
            core.updatePos();
    }
}


static void GrapheneSettingsHandler_ClearAll(ImGuiContext *ctx, ImGuiSettingsHandler *) {
    LOG_INFO("GrapheneSettingsHandler: Clear all");
}

static void GrapheneSettingsHandler_ApplyAll(ImGuiContext *ctx, ImGuiSettingsHandler *) {
    LOG_INFO("GrapheneSettingsHandler: Apply all");
}

static void *GrapheneSettingsHandler_ReadOpen(ImGuiContext *, ImGuiSettingsHandler *, const char *name) {
    LOG_INFO("GrapheneSettingsHandler: Read open");
}

static void GrapheneSettingsHandler_ReadLine(ImGuiContext *, ImGuiSettingsHandler *, void *entry, const char *line) {
    LOG_INFO("GrapheneSettingsHandler: Read line");

    bool maximized;
    if (sscanf(line, "Maximized=%d", &maximized) == 1) {
        LOG_INFO(std::string("GrapheneSettingsHandler: Maximized: ") + (maximized ? "true" : "false"));
        return;
    }

    int sizeX, sizeY;
    if (sscanf(line, "Size=%d,%d", &sizeX, &sizeY) == 2) {
        std::cerr << "size\n";
        LOG_INFO(std::string("GrapheneSettingsHandler: Size: ") + std::to_string(sizeX) + ", " + std::to_string(sizeY));
        return;
    }

}

static void GrapheneSettingsHandler_WriteAll(ImGuiContext *ctx, ImGuiSettingsHandler *handler, ImGuiTextBuffer *buf) {
    LOG_INFO("GrapheneSettingsHandler: Write all");

    buf->reserve(600);
    buf->appendf("[Graphene][Config]\n");
    buf->appendf("Maximized=1\n");
    buf->appendf("Size=1920,1080\n");
    buf->append("\n");

}

std::string file = "Graphene.ini";

int main() {

    Gui::init();

    ImGui::GetIO().IniFilename = file.c_str();

    Graphene::Core core;

    bool updateGraphAsync = true;
    // Multi-threading is temporarily disabled because it leads to crash
//    bool endBackgroundUpdateLoop = false;
//    std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(updateGraphAsync), std::ref(endBackgroundUpdateLoop));
//    backgroundUpdateThread.detach();

    {
        ImGuiContext &g = *ImGui::GetCurrentContext();
        ImGuiSettingsHandler ini_handler;
        ini_handler.TypeName = "Graphene";
        ini_handler.TypeHash = ImHashStr("Graphene");
        ini_handler.ClearAllFn = GrapheneSettingsHandler_ClearAll;
        ini_handler.ReadOpenFn = GrapheneSettingsHandler_ReadOpen;
        ini_handler.ReadLineFn = GrapheneSettingsHandler_ReadLine;
        ini_handler.ApplyAllFn = GrapheneSettingsHandler_ApplyAll;
        ini_handler.WriteAllFn = GrapheneSettingsHandler_WriteAll;
        g.SettingsHandlers.push_back(ini_handler);
    }

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
        if (updateGraphAsync) core.updatePos();

        core.updateRateCounter.updateWithoutCount();
    }

//    endBackgroundUpdateLoop = true;

    Gui::cleanup();
}
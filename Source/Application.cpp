#include "editor/Include.h"
#include "core/Include.h"
#include "editor/panels/graphview/GraphView/GraphView.hpp"
#include "editor/panels/graphview/GraphView.hpp"

int main() {
    gph::Core core;

    for (int i = 0; i < 100; i++)
        core.getGraphObj()->newVertex();

    if (graphene::editor::frameworkInit(graphene::editor::frameworkConfig())) {
        while (graphene::editor::newFrame()) {
            gph::GraphView::show(&core);
            ImGui::ShowDemoWindow();
            graphene::editor::endFrame();
        }
    }
}
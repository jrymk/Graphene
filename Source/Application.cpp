#include "editor/Include.h"
#include "core/Include.h"
#include "editor/panels/graphview/GraphView/GraphView.hpp"

int main() {
    gph::Core core;

    for (int i = 0; i < 10; i++)
        core.getGraphObj()->newVertex();

    while (graphene::editor::frameworkInit(graphene::editor::frameworkConfig())) {
        while (graphene::editor::newFrame()) {
            ImGui::ShowDemoWindow();

            gph::GraphView::show(&core);

            graphene::editor::endFrame();
        }
    }
}
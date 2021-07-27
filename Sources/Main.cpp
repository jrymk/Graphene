#include <stdio.h>
#include <string>
#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <Logging/Logging.hpp>
#include <Editor/Logs.hpp>
#include <Objects/Uuid.hpp>
#include <Interface/Interface.hpp>
#include <Editor/DebugGraphView.hpp>
// #include <Editor/Logs.hpp>

#include <thread>

void updateGraphLoop(gfn::core::Core &core, bool &update, bool &end) {
    while (!end) {
        if (update) {
            // std::cerr << ".";
            core.coreCycle();
        }
    }
}

int main() {
    std::cerr << "Hello world\n";

    gfn::window::launchWindow();
    gfn::interface::Interface interface;
    gfn::core::Core core(&interface);

    bool updateGraphAsync = true;
    bool endBackgroundUpdateLoop = false;

    std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(updateGraphAsync),
                                       std::ref(endBackgroundUpdateLoop));
    backgroundUpdateThread.detach();

    while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
        // core.coreCycle(&interface);
        // std::cerr << "|";
        gfn::window::preFrame();
        ImGui::ShowDemoWindow();

        // gfn::editor::logs::show();

        //auto stat = interface.properties.getOutpaceStatistics();
        //static int v;

        //if (!interface.properties.isReadBufferRead())
        //    v = stat.second - stat.first;

        ImGui::Begin("Command centre");

        /*ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(255, 255, 255, 255));
        if (v > 0)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(200, 255, 200, 255));
        else if (v == 0)
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 255, 200, 255));
        else
            ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 200, 200, 255));

        ImGui::SliderInt("Thread outpace", &v, -10000000, 10000000, "%d",
                         ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

        ImGui::PopStyleColor(2);*/

        static float _c1 = 2.0f;
        static float _c2 = 1.0f;
        static float _c3 = 1.0f;
        static float _c4 = 0.1f;
        static float _c5 = 0.1f;
        static float _c6 = 0.001f;
        ImGui::SliderFloat("c1", &_c1, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("c2", &_c2, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("c3", &_c3, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("c4", &_c4, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("c5", &_c5, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("c6", &_c6, 0.000001, 10.0, "%f", ImGuiSliderFlags_Logarithmic);
        interface.configs.getWrite()->c1 = _c1;
        interface.configs.getWrite()->c2 = _c2;
        interface.configs.getWrite()->c3 = _c3;
        interface.configs.getWrite()->c4 = _c4;
        interface.configs.getWrite()->c5 = _c5;
        interface.configs.getWrite()->c6 = _c6;

        // if (ImGui::Button("New vertex"))
        // core.parser.execute("graph new vertex"); // No, you can not run this here
        ImGui::End();

        gfn::editor::logs::show(interface.logBuffer.getRead());
        interface.logBuffer.readDone();

        gfn::editor::debuggraphview::show(interface.userprops.getRead());
        interface.userprops.readDone();

        interface.configs.writeDone();

        gfn::window::postFrame();
    }
    endBackgroundUpdateLoop = true;
    gfn::window::closeWindow();
    return 0;
}
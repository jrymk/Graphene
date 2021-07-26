#include <stdio.h>
#include <string>
#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <Core/Logging/Logging.hpp>
#include <Editor/Logs.hpp>
#include <Core/Objects/Uuid.hpp>
#include <Interface/Interface.hpp>
#include <Editor/DebugGraphView.hpp>
// #include <Editor/Logs.hpp>

#include <thread>

void updateGraphLoop(gfn::core::Core& core, gfn::interface::Interface& interface, bool& update, bool& end) {
	while (!end) {
		if (update) {
			// std::cerr << ".";
			core.coreCycle(&interface);
		}
	}
}

int main() {
	std::cerr << "Hello world\n";

	gfn::window::launchWindow();
	gfn::interface::Interface interface;
	gfn::core::Core core;

	bool updateGraphAsync = true;
	bool endBackgroundUpdateLoop = false;

	std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(interface), std::ref(updateGraphAsync),
									   std::ref(endBackgroundUpdateLoop));
	backgroundUpdateThread.detach();

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		// core.coreCycle(&interface);
		// std::cerr << "|";
		gfn::window::preFrame();
		ImGui::ShowDemoWindow();

		// gfn::editor::logs::show();

		auto stat = interface.properties.getOutpaceStatistics();
		static int v;

		if (!interface.properties.isReadBufferRead())
			v = stat.second - stat.first;

		ImGui::Begin("Command centre");

		ImGui::PushStyleColor(ImGuiCol_SliderGrab, IM_COL32(255, 255, 255, 255));
		if (v > 0)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(200, 255, 200, 255));
		else if (v == 0)
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 255, 200, 255));
		else
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(255, 200, 200, 255));

		ImGui::SliderInt("Thread outpace", &v, -10000000, 10000000, "%d",
						 ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoInput);

		ImGui::PopStyleColor(2);

		// if (ImGui::Button("New vertex"))
		// core.parser.execute("graph new vertex"); // No, you can not run this here
		ImGui::End();

		gfn::editor::logs::show(interface.logBuffer.getReadBuffer());

		gfn::editor::debuggraphview::show(interface.properties.getReadBuffer());
		// read done
		interface.properties.readDone();
		interface.logBuffer.readDone();

		gfn::window::postFrame();
	}
	endBackgroundUpdateLoop = true;
	gfn::window::closeWindow();
	return 0;
}
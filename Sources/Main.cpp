#include <string>
#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Objects/Uuid.hpp>
#include <Interface/Interface.hpp>
#include <Editor/Logs.hpp>

#include <thread>

void updateGraphLoop(gfn::core::Core& core, gfn::interface::Interface& interface, bool& update, bool& end) {
	while (!end) {
		if (update) {
            std::cerr << "Started\n";
            core.coreCycle(&interface);
            std::cerr << "Ended\n";
		}
	}
}

int main() {
	gfn::window::launchWindow();
	gfn::interface::Interface interface;
	gfn::core::Core core;

	bool updateGraphAsync = true;
	bool endBackgroundUpdateLoop = false;

	std::thread backgroundUpdateThread(updateGraphLoop, std::ref(core), std::ref(interface), std::ref(updateGraphAsync),
									   std::ref(endBackgroundUpdateLoop));
	backgroundUpdateThread.detach();

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		//core.coreCycle(&interface);

		gfn::window::preFrame();
		ImGui::ShowDemoWindow();

		//gfn::editor::logs::show();

		ImGui::Begin("Command centre");
		//if (ImGui::Button("New vertex"))
			//core.parser.execute("graph new vertex"); //No, you can not run this here
		ImGui::End();

		//gfn::interface::Content content;
		//interface.buffer.get(content, true);

		gfn::window::postFrame();
		//core.pendingOutput = true;
	}
	endBackgroundUpdateLoop = true;
	gfn::window::closeWindow();
	return 0;
}
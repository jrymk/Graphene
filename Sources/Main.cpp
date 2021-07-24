#include <string>
#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Objects/Uuid.hpp>
#include <Interface/Interface.hpp>
#include <Editor/Logs.hpp>

int main() {
	gfn::window::launchWindow();
	gfn::interface::Interface interface;
	gfn::core::Core core;

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		core.coreCycle(&interface);

		gfn::window::preFrame();
		ImGui::ShowDemoWindow();

		gfn::editor::logs::show();

		ImGui::Begin("Command center");
		if (ImGui::Button("New vertex"))
			core.parser.execute("graph new vertex");
		ImGui::End();

		gfn::interface::Content content;
		interface.buffer.get(content);

		gfn::window::postFrame();
		core.pendingOutput = true;
	}

	return 0;
}
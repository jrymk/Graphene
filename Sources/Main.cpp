#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Objects/Uuid.hpp>

int main() {
	// gfn::window::launchWindow();
	gfn::core::Core core;

	while (true) {
		gfn::core::parser::Command cmd;
		std::getline(std::cin, cmd);
		core.parser.execute(cmd);
	}
	/*while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		gfn::window::preFrame();

		ImGui::ShowDemoWindow();

		gfn::window::postFrame();
	}*/

	return 0;
}
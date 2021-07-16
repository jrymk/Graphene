#include <iostream>
#include "System/Window/WindowManager.hpp"
#include "System/Logging/Logging.hpp"

int main() {
	gfn::window::windowConfig windowConfig;
	gfn::window::launchWindow(windowConfig);

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		gfn::window::preFrame();

		ImGui::ShowDemoWindow();

		gfn::window::postFrame();
	}

	return 0;
}
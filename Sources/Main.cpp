#include <iostream>
#include <Core/Data/Structure/Structure.hpp>
#include <System/Window/WindowManager.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Data/Properties/Uuid.hpp>

int main() {
	gfn::window::launchWindow();

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		gfn::window::preFrame();

		ImGui::ShowDemoWindow();

		gfn::window::postFrame();

		// std::cout << gfn::window::config::maximize << "\n";
	}

	return 0;
}
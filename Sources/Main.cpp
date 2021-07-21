#include <iostream>
#include <Core/Core.hpp>
#include <System/Window/WindowManager.hpp>
#include <System/Logging/Logging.hpp>
#include <Core/Objects/Uuid.hpp>

int main() {
	gfn::window::launchWindow();

	while (!glfwWindowShouldClose(gfn::window::glfwWindow)) {
		gfn::window::preFrame();

		ImGui::ShowDemoWindow();

		gfn::window::postFrame();
	}

	return 0;
}
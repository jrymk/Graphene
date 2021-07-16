#pragma once

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace gfn::window {
extern GLFWwindow* glfwWindow;

void preFrame() {
	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void postFrame() {
	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(glfwWindow);
}
} // namespace gfn::window
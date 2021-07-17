#pragma once

#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <System/Window/WindowConfigs.hpp>
#include <System/Window/Frame.hpp>

namespace gfn::window {
static void glfw_error_callback(int error, const char* description) {
	std::cerr << "glfw error: " << error << ": " << description << "\n";
}

GLFWwindow* glfwWindow = nullptr;

bool launchWindow() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return false;

	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glfwWindow = glfwCreateWindow(config::width, config::height, "Graphene", NULL, NULL);

	if (glfwWindow == NULL)
		return false;

	glfwMakeContextCurrent(glfwWindow);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize OpenGL loader!\n";
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;	  // Enable Window Docking
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::GetIO().ConfigDockingAlwaysTabBar = true;

	ImGui::StyleColorsLight();

	ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::GetIO().IniFilename = "Graphene.ini";
	ImGui::GetIO().Fonts->AddFontFromFileTTF(".\\Barlow-Regular.ttf", 16.0f);

	return true;
}

void closeWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}
} // namespace gfn::window
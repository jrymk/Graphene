#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#include "include.h"
#include "../utils/exceptionHandler.hpp"
#include "../graphene/include.h"
#include "themes.hpp"

namespace Gui {

	GLFWwindow* window;
	ImFont* vertexTextFont;
	bool showDemoWindow = false;

	static void HelpMarker(const char* desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered()) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void glfwErrorCallback(int error, const char* description) {
		ERR("GLFW Error: " + std::to_string(error) + std::string(description));
	}

	int init() {

		glfwSetErrorCallback(glfwErrorCallback);
		if (!glfwInit())
			return 1;

		glfwWindowHint(GLFW_MAXIMIZED, true);

		window = glfwCreateWindow(1280, 720, u8"Graphene pre-α", NULL, NULL);
		if (window == NULL)
			return 2;

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return 3;

		// disable vsync
		glfwSwapInterval(0);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// multi viewport is disabled, so windows don't get hidden by the main window
		//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGui::GetIO().ConfigViewportsNoDecoration = true;
		ImGui::GetIO().ConfigViewportsNoDefaultParent = true;
		ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;


		ImGui::StyleColorsLight();

		setTheme(false);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");


		ImFontConfig config;
		config.OversampleH = 2;
		config.OversampleV = 1;
		config.MergeMode = false;
		// default font for Latin characters (default: Manrope)
		ImGui::GetIO().Fonts->AddFontFromFileTTF("./Manrope-Regular.ttf", 16.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesDefault());

		config.MergeMode = true;
		// fallback font for Chinese characters (default: Microsoft JhengHei UI Regular)
		ImGui::GetIO().Fonts->AddFontFromFileTTF("./msjh.ttf", 16.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

		// mono font for graph display
		// TODO: remove it
		vertexTextFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 36.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
		config.MergeMode = false;

		ImGui::CreateContext();

	}

	
	bool shouldClose() {
		return glfwWindowShouldClose(window);
	}


	void initFrame() {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	}

	void render() {

		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);

		ImGui::Render();

		int displayW, displayH;
		glfwGetFramebufferSize(Gui::window, &displayW, &displayH);

		glViewport(0, 0, displayW, displayH);

		glClearColor(80, 80, 80, 255);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

		glfwSwapBuffers(Gui::window);

	}

	void cleanup() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();

		glfwDestroyWindow(Gui::window);
		glfwTerminate();

	}

}
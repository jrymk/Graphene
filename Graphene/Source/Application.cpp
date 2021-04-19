#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ExceptionHandler.hpp"


static void glfw_error_callback(int error, const char* description) {
	ERR("GLFW Error: " + std::to_string(error) + std::string(description));
}

static void useStyle() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->FrameRounding = 6.0f;
	style->GrabRounding = 4.0f;
	style->PopupRounding = 4.0f;
	style->ChildRounding = 4.0f;
	style->TabRounding = 6.0f;
	style->WindowRounding = 8.0f;

	style->Colors[ImGuiCol_WindowBg].w = 1.0f;
}

int main() {

	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	glfwWindowHint(GLFW_MAXIMIZED, true);

	GLFWwindow* window = glfwCreateWindow(1280, 720, u8"Graphene α", NULL, NULL);
	if (window == NULL)
		return 2;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return 3;


	//glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	// multi viewport is disabled, so windows don't get hidden by the main window
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigViewportsNoDecoration = true;
	io.ConfigViewportsNoDefaultParent = true;
	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsLight();

	useStyle();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImFontConfig config;
	config.OversampleH = 1;
	config.OversampleV = 1;
	config.PixelSnapH = true;
	config.MergeMode = false;
	io.Fonts->AddFontFromFileTTF("./msjh.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->AddFontFromFileTTF("./Manrope-Regular.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontFromFileTTF("./NotoSansTC-Regular.otf", 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
	io.Fonts->AddFontFromFileTTF("./Roboto-Medium.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());

	bool show_demo_window = true;
	bool show_another_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin(u8"你好", 0, ImGuiWindowFlags_NoCollapse);                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window, ImGuiWindowFlags_NoCollapse);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}


		ImGui::Render();

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);

		glViewport(0, 0, display_w, display_h);

		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
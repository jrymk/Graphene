#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ExceptionHandler.hpp"
#include "imgui/addons/imguivariouscontrols/imguivariouscontrols.h"


static void glfw_error_callback(int error, const char* description) {
	ERR("GLFW Error: " + std::to_string(error) + std::string(description));
}

static void useStyle() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->FrameRounding = 6.0f;
	style->GrabRounding = 6.0f;
	style->PopupRounding = 4.0f;
	style->ChildRounding = 4.0f;
	style->TabRounding = 6.0f;
	style->WindowRounding = 8.0f;

	style->WindowPadding = { 8.0f, 8.0f };
	style->FramePadding = { 8.0f, 4.0f };
	style->ItemSpacing = { 8.0f, 8.0f };
	style->FrameBorderSize = 0.0f;

	style->DisplaySafeAreaPadding.x = 8.0f;
	style->DisplaySafeAreaPadding.y = 8.0f;

	style->Colors[ImGuiCol_WindowBg].w = 1.0f;
	ImVec4* colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.30f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.76f, 0.44f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(1.00f, 0.80f, 0.53f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.67f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.71f, 0.32f, 0.82f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.59f, 0.00f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.67f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.81f, 0.56f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.00f, 0.64f, 0.18f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.67f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.67f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.67f, 0.26f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.80f, 0.51f, 0.14f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.80f, 0.51f, 0.14f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.67f, 0.26f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.67f, 0.26f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.87f, 0.80f, 0.71f, 0.93f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.67f, 0.26f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.95f, 0.69f, 0.31f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.78f, 0.78f, 0.77f, 0.98f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.90f, 0.76f, 0.57f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.67f, 0.26f, 0.46f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.98f, 0.89f, 0.78f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.64f, 0.63f, 0.57f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.74f, 0.73f, 0.68f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.67f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.98f, 0.67f, 0.26f, 0.95f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.67f, 0.26f, 0.80f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);



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


	glfwSwapInterval(1);

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
	glfwWindowHint(GLFW_SAMPLES, 4);

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	ImFontConfig config;
	config.OversampleH = 4;
	config.OversampleV = 4;
	config.MergeMode = false;
	// default font for Latin characters (default: Manrope)
	io.Fonts->AddFontFromFileTTF("./Manrope-Regular.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());

	config.MergeMode = true;
	// fallback font for Chinese characters (default: Microsoft JhengHei UI Regular)
	io.Fonts->AddFontFromFileTTF("./msjh.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
	//io.Fonts->AddFontFromFileTTF("./NotoSansTC-Regular.otf", 18.0f, &config, io.Fonts->GetGlyphRangesChineseFull());
	//io.Fonts->AddFontFromFileTTF("./Roboto-Medium.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());
	//io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 18.0f, &config, io.Fonts->GetGlyphRangesDefault());

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

			ImGui::Begin(u8"Graphene", 0, ImGuiWindowFlags_NoCollapse);

			static ImVector<ImVec2> points;
			static ImVec2 scrolling(0.0f, 0.0f);
			static bool opt_enable_grid = true;
			static bool opt_enable_context_menu = true;
			static bool adding_line = false;

			ImGui::Text("Mouse Left: drag to add lines,\nMouse Right: drag to scroll, click for context menu.");

			// Typically you would use a BeginChild()/EndChild() pair to benefit from a clipping region + own scrolling.
			// Here we demonstrate that this can be replaced by simple offsetting + custom drawing + PushClipRect/PopClipRect() calls.
			// To use a child window instead we could use, e.g:
			//      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
			//      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
			//      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true, ImGuiWindowFlags_NoMove);
			//      ImGui::PopStyleColor();
			//      ImGui::PopStyleVar();
			//      [...]
			//      ImGui::EndChild();

			// Using InvisibleButton() as a convenience 1) it will advance the layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
			ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
			ImVec2 canvas_sz = ImGui::GetContentRegionAvail();   // Resize canvas to what's available
			if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
			if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
			ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

			// Draw border and background color
			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
			draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

			// Add first and second point
			if (is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				points.push_back(mouse_pos_in_canvas);
				points.push_back(mouse_pos_in_canvas);
				adding_line = true;
			}
			if (adding_line) {
				points.back() = mouse_pos_in_canvas;
				if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
					adding_line = false;
			}

			// Pan (we use a zero mouse threshold when there's no context menu)
			// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
			const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan)) {
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}

			// Context menu (under default mouse threshold)
			ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
			if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
				ImGui::OpenPopupOnItemClick("context");
			if (ImGui::BeginPopup("context")) {
				if (adding_line)
					points.resize(points.size() - 2);
				adding_line = false;
				if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
				if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }
				ImGui::EndPopup();
			}

			// Draw grid + all lines in the canvas
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);
			if (opt_enable_grid) {
				const float GRID_STEP = 64.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
				for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
					draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
			}
			for (int n = 0; n < points.Size; n += 2)
				draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
			draw_list->PopClipRect();

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin(u8"中文標題", 0, ImGuiWindowFlags_NoCollapse);

			ImGui::Text(u8"我是中文文字。");
			ImGui::Checkbox(u8"顯示演示視窗", &show_demo_window);

			ImGui::SliderFloat(u8"浮點數", &f, 0.0f, 1.0f);
			ImGui::ColorEdit3(u8"刷新顏色", (float*)&clear_color);

			if (ImGui::Button(u8"按鈕"))
				counter++;
			ImGui::SameLine();
			ImGui::Text(u8"計數器 = %d", counter);

			ImGui::Text(u8"應用程式平均每幀 %.3f 毫秒 (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
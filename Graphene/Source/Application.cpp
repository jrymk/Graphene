#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ExceptionHandler.hpp"
#include "imgui/addons/imguinodegrapheditor/imguinodegrapheditor.h"
#include "graphene/Graphene.h"


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

	GLFWwindow* window = glfwCreateWindow(1280, 720, u8"Graphene pre-α", NULL, NULL);
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
	config.OversampleH = 2;
	config.OversampleV = 1;
	config.MergeMode = false;
	// default font for Latin characters (default: Manrope)
	io.Fonts->AddFontFromFileTTF("./Manrope-Regular.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesDefault());

	config.MergeMode = true;
	// fallback font for Chinese characters (default: Microsoft JhengHei UI Regular)
	io.Fonts->AddFontFromFileTTF("./msjh.ttf", 16.0f, &config, io.Fonts->GetGlyphRangesChineseFull());

	config.MergeMode = false;
	ImFont* vertexTextFont = io.Fonts->AddFontFromFileTTF("./JetBrainsMono-Regular.ttf", 36.0f, &config, io.Fonts->GetGlyphRangesDefault());



	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<std::vector<bool>> graphene_graph;

	int graphene_vertices = 5;

	bool use_normalized = true;

	Graphene::Graph graph;
	Graphene::Core core(graph);


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		static bool show_demo_window;
		static bool graphene_live_update;

		{
			ImGui::Begin("Toolbar");

			ImGui::Checkbox("Enable live update", &graphene_live_update);

			ImGui::SameLine();

			if (ImGui::Button("Update graph") || graphene_live_update) {
				core.updatePos();
			}

		}

		{
			ImGui::Begin("Vertex Info");

			static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Hideable;

			ImVec2 outer_size = ImVec2(0.0f, 0.0f);
			if (ImGui::BeginTable("table_scrollx", 5, flags, outer_size)) {

				ImGui::TableSetupScrollFreeze(1, 1);
				ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()
				ImGui::TableSetupColumn("PosX");
				ImGui::TableSetupColumn("PosY");
				ImGui::TableSetupColumn("NormX");
				ImGui::TableSetupColumn("MormY");

				ImGui::TableHeadersRow();
				for (int vertex = 0; vertex < graph.totalVertex; vertex++) {
					ImGui::TableNextRow();

					for (int column = 0; column < 5; column++) {

						if (!ImGui::TableSetColumnIndex(column) && column > 0)
							continue;

						switch (column) {
						case 0:
							ImGui::Text("%d", vertex);
							break;
						case 1:
							ImGui::Text("%f", graph.verticies[vertex].coord.x);
							break;
						case 2:
							ImGui::Text("%f", graph.verticies[vertex].coord.y);
							break;
						case 3:
							ImGui::Text("%f", graph.verticies[vertex].normalized.x);
							break;
						case 4:
							ImGui::Text("%f", graph.verticies[vertex].normalized.y);
							break;

						}

					}

				}
				ImGui::EndTable();
			}

			ImGui::End();
		}


		{
			ImGui::Begin("Input");

			ImGui::BeginTabBar("input_method");

			if (ImGui::BeginTabItem("Raw input")) {
				static char text[65536] = "";
				static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

				if (ImGui::Button("Update")) {
					std::stringstream ss;
					ss << text;
					int v, e;
					ss >> v >> e;
					std::vector<std::pair<std::pair<int, int>, bool>> temp_graph;
					for (int i = 0; i < e; i++) {
						int a, b;
						ss >> a >> b;
						temp_graph.push_back({ { a, b }, false });
					}
					graph.initialize(v, e, temp_graph);
				}

				ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, -FLT_MIN), flags);

				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("Adjacency Matrix")) {
				bool check_update = false;
				int prev_vertices = graphene_vertices;

				ImGui::SliderInt("Vertices", &graphene_vertices, 0, 20, "%d", 0);
				if (prev_vertices != graphene_vertices)
					check_update = true;

				graphene_graph.resize(graphene_vertices);
				for (int i = 0; i < graphene_vertices; i++)
					graphene_graph[i].resize(graphene_vertices);


				if (ImGui::BeginTable("checkbocGridTable", graphene_vertices + 1, 1 << 13, ImVec2(0, 0), 0.0)) {
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					for (int column = 0; column < graphene_vertices; column++) {
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(column).c_str());
					}
					for (int row = 0; row < graphene_vertices; row++) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::Text(std::to_string(row).c_str());

						for (int column = 0; column < graphene_vertices; column++) {
							ImGui::TableNextColumn();

							bool check = graphene_graph[row][column];
							bool prevCheck = check;
							ImGui::Checkbox((std::string("##") + std::to_string(row) + std::string(",") + std::to_string(column)).c_str(), &check);
							graphene_graph[row][column] = check;
							graphene_graph[column][row] = check;

							if (prevCheck != check)
								check_update = true;
						}

					}
					ImGui::EndTable();
				}

				ImGui::EndTabItem();


				if (check_update) {
					int tickCount = 0;
					std::vector<std::pair<std::pair<int, int>, bool>> temp_graph;

					for (int i = 0; i < graphene_vertices; i++) {
						for (int j = 0; j < graphene_vertices; j++) {
							if (graphene_graph[i][j]) {
								if (i >= j) {
									tickCount++;
									temp_graph.push_back({ {i, j}, false });
								}
							}
						}
					}

					graph.initialize(graphene_vertices, tickCount, temp_graph);
				}

			}

			ImGui::EndTabBar();

			ImGui::End();
		}


		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{

			ImGui::Begin(u8"Graphene", 0, ImGuiWindowFlags_NoCollapse);

			static float zoomLevel = 800.0f;
			static float canvasMargin = 50.0f;

			ImGui::SliderFloat("Zoom level", &zoomLevel, 50.0f, 2000.0f);
			ImGui::SameLine();
			ImGui::Checkbox("Normalized", &use_normalized);

			static ImVector<ImVec2> points;
			static ImVec2 scrolling(0.0f, 0.0f);
			static bool opt_enable_grid = true;
			static bool opt_enable_context_menu = true;
			static bool adding_line = false;

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

			// This will catch our interactions
			ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Held
			const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y); // Lock scrolled origin
			const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x, io.MousePos.y - origin.y);

			// Draw grid + all lines in the canvas
			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			draw_list->PushClipRect(canvas_p0, canvas_p1, true);

			// Draw border and background color
			draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(0, 0, 0, 255), 8.0f, 0);
			draw_list->AddRectFilled(ImVec2(origin.x - canvasMargin, origin.y - canvasMargin),
				ImVec2(origin.x + zoomLevel + canvasMargin, origin.y + zoomLevel + canvasMargin), IM_COL32(50, 50, 50, 255), 8.0f, 0);

			// Pan (we use a zero mouse threshold when there's no context menu)
			// You may decide to make that threshold dynamic based on whether the mouse is hovering something etc.
			const float mouse_threshold_for_pan = opt_enable_context_menu ? -1.0f : 0.0f;
			if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan)) {
				scrolling.x += io.MouseDelta.x;
				scrolling.y += io.MouseDelta.y;
			}

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


			if (opt_enable_grid) {
				const float GRID_STEP = zoomLevel / 10.0f;
				for (float x = fmodf(scrolling.x, GRID_STEP); x <= canvas_sz.x; x += GRID_STEP) {
					if (canvas_p0.x + x >= origin.x && canvas_p0.x + x <= origin.x + zoomLevel)
						draw_list->AddLine(ImVec2(canvas_p0.x + x, max(canvas_p0.y, origin.y)),
							ImVec2(canvas_p0.x + x, min(canvas_p1.y, origin.y + zoomLevel)), IM_COL32(90, 90, 90, 255), 1.0f);
				}
				for (float y = fmodf(scrolling.y, GRID_STEP); y <= canvas_sz.y; y += GRID_STEP) {
					if (canvas_p0.y + y >= origin.y && canvas_p0.y + y <= origin.y + zoomLevel)
						draw_list->AddLine(ImVec2(max(canvas_p0.x, origin.x), canvas_p0.y + y),
							ImVec2(min(canvas_p1.x, origin.x + zoomLevel), canvas_p0.y + y), IM_COL32(90, 90, 90, 255), 1.0f);
				}

				draw_list->AddRect(ImVec2(origin.x, origin.y),
					ImVec2(origin.x + zoomLevel, origin.y + zoomLevel), IM_COL32(90, 90, 90, 255), 5.0f, 0, 2.0f);
			}

			float x_max = -1000000000.0;
			float x_min = 1000000000.0;
			float y_max = -1000000000.0;
			float y_min = 1000000000.0;

			for (int i = 0; i < graph.totalVertex; i++) {
				x_max = max(x_max, graph.verticies[i].coord.x);
				x_min = min(x_min, graph.verticies[i].coord.x);
				y_max = max(y_max, graph.verticies[i].coord.y);
				y_min = min(y_min, graph.verticies[i].coord.y);
			}


			for (int i = 0; i < graph.totalEdge; i++) {
				if (use_normalized) {
					draw_list->AddLine(
						ImVec2(origin.x + ((graph.allEdges[i].startingVertex->coord.x - x_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel),
							origin.y + ((graph.allEdges[i].startingVertex->coord.y - y_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel)),
						ImVec2(origin.x + ((graph.allEdges[i].endingVertex->coord.x - x_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel),
							origin.y + ((graph.allEdges[i].endingVertex->coord.y - y_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel)),
						IM_COL32(200, 200, 200, 255), 5.0f);
				}
				else {
					draw_list->AddLine(
						ImVec2(origin.x + (((graph.allEdges[i].startingVertex->coord.x + 2147483647.0f) / 4294967295.0f) * zoomLevel),
							(((graph.allEdges[i].startingVertex->coord.y + 2147483647.0f) / 4294967295.0f) * zoomLevel)),
						ImVec2((((graph.allEdges[i].endingVertex->coord.x + 2147483647.0f) / 4294967295.0f) * zoomLevel),
							(((graph.allEdges[i].endingVertex->coord.y + 2147483647.0f) / 4294967295.0f) * zoomLevel)),
						IM_COL32(200, 200, 200, 255), 5.0f);
				}
			}

			for (int i = 0; i < graph.totalVertex; i++) {
				graph.verticies[i].normalized.x = (graph.verticies[i].coord.x - x_min) / max((x_max - x_min), (y_max - y_min));
				graph.verticies[i].normalized.y = (graph.verticies[i].coord.y - y_min) / max((x_max - x_min), (y_max - y_min));

				if (use_normalized) {
					draw_list->AddCircleFilled(ImVec2(origin.x + ((graph.verticies[i].coord.x - x_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel),
						origin.y + ((graph.verticies[i].coord.y - y_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel)), 20.0f, IM_COL32(255, 211, 0, 255));
				}
				else {
					draw_list->AddCircleFilled(ImVec2(origin.x + (((graph.verticies[i].coord.x + 2147483647.0f) / 4294967295.0f) * zoomLevel),
						origin.y + (((graph.verticies[i].coord.y + 2147483647.0f) / 4294967295.0f) * zoomLevel)), 20.0f, IM_COL32(255, 211, 0, 255));

				}
				ImGui::PushFont(vertexTextFont);
				float font_size = ImGui::GetFontSize() * std::to_string(i).size() / 2;

				if (use_normalized) {
					draw_list->AddText(vertexTextFont, 36.0f,
						ImVec2(origin.x + ((graph.verticies[i].coord.x - x_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel) - font_size + (font_size / 2),
							origin.y + ((graph.verticies[i].coord.y - y_min) / max((x_max - x_min), (y_max - y_min)) * zoomLevel) - 18.0f),
						IM_COL32(15, 15, 15, 255), std::to_string(i).c_str(), 0, 0.0f, 0);
				}
				else {
					draw_list->AddText(vertexTextFont, 36.0f,
						ImVec2(origin.x + (((graph.verticies[i].coord.x + 2147483647.0f) / 4294967295.0f) * zoomLevel) - font_size + (font_size / 2),
							origin.y + (((graph.verticies[i].coord.y + 2147483647.0f) / 4294967295.0f) * zoomLevel) - 18.0f),
						IM_COL32(15, 15, 15, 255), std::to_string(i).c_str(), 0, 0.0f, 0);
				}
				ImGui::PopFont();
			}

			draw_list->PopClipRect();

			ImGui::End();
		}

		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin(u8"Dear ImGUI", 0, ImGuiWindowFlags_NoCollapse);

			ImGui::Text(u8"Graphene pre-alpha");
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

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

		glfwSwapBuffers(window);
	}


	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}
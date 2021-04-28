#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ExceptionHandler.hpp"
#include "graphene/Graphene.h"


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

static void glfw_error_callback(int error, const char* description) {
	ERR("GLFW Error: " + std::to_string(error) + std::string(description));
}

static void useStyle(bool dark) {
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

	if (!dark) {
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
	else {
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.42f, 0.16f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.85f, 0.26f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.85f, 0.26f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.42f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.85f, 0.26f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.88f, 0.77f, 0.24f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.85f, 0.26f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.98f, 0.85f, 0.26f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.85f, 0.26f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.82f, 0.06f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.98f, 0.85f, 0.26f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.85f, 0.26f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.85f, 0.26f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.49f, 0.43f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.63f, 0.10f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.63f, 0.10f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.85f, 0.26f, 0.20f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.85f, 0.26f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.85f, 0.26f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.51f, 0.18f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.85f, 0.26f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.59f, 0.20f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.13f, 0.07f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.37f, 0.14f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.85f, 0.26f, 0.79f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 0.40f, 0.90f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.45f, 1.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.85f, 0.26f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.00f, 0.45f, 1.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.85f, 0.26f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

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


	glfwSwapInterval(0);

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

	useStyle(false);

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

	static bool show_demo_window;
	static bool graphene_live_update = true;

	ImGui::CreateContext();


	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		{
			ImGui::Begin("Toolbar");

			ImGui::Checkbox("Enable live update", &graphene_live_update);

			ImGui::SameLine();

			if (ImGui::Button("Update graph") || graphene_live_update)
				core.updatePos();

			if (ImGui::Button("Reset to default")) {
				core.m_c1 = 2;
				core.m_c2 = 1;
				core.m_c3 = 1;
				core.m_c4 = 0.1;
			}

			ImGui::Checkbox("Repel on adjacent vertices", &core.not_adjacent_repel);

			ImGui::SliderFloat("C1", &core.m_c1, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			HelpMarker("Attraction force factor");

			ImGui::SliderFloat("C2", &core.m_c2, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			HelpMarker("Attraction log");

			ImGui::SliderFloat("C3", &core.m_c3, 0.01f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			HelpMarker("Repel factor");

			ImGui::SliderFloat("C4", &core.m_c4, 0.001f, 10000.0f, "%f", ImGuiSliderFlags_Logarithmic);
			ImGui::SameLine();
			HelpMarker("Factor");
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
				ImGui::TableSetupColumn("ResultX");
				ImGui::TableSetupColumn("ResultY");

				ImGui::TableHeadersRow();
				for (int vertex = 0; vertex < graph.vertexCount; vertex++) {
					ImGui::TableNextRow();

					for (int column = 0; column < 5; column++) {

						if (!ImGui::TableSetColumnIndex(column) && column > 0)
							continue;

						switch (column) {
						case 0:
							ImGui::Text("%d", vertex);
							break;
						case 1:
							ImGui::Text("%f", graph.vertices[vertex]->coord.x);
							break;
						case 2:
							ImGui::Text("%f", graph.vertices[vertex]->coord.y);
							break;
						case 3:
							ImGui::Text("%f", graph.vertices[vertex]->resultForce.x);
							break;
						case 4:
							ImGui::Text("%f", graph.vertices[vertex]->resultForce.y);
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
					graph.importEdges(v, temp_graph);
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
								tickCount++;
								temp_graph.push_back({ {i, j}, false });
							}
						}
					}

					graph.importEdges(graphene_vertices, temp_graph);
				}

			}

			ImGui::EndTabBar();

			ImGui::Checkbox(u8"Show demo window", &show_demo_window);

			ImGui::End();
		}


		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);



		{
			ImGui::Begin(u8"Graph View", 0, ImGuiWindowFlags_NoCollapse);

			bool contextMenuEnabled = false;
			static bool autoZoomPan = true;
			static bool showGrid = true;
			static float zoomOffset = 1.0f;


			ImGui::Checkbox("Show grid", &showGrid);
			ImGui::SameLine();
			ImGui::Checkbox("Auto adjust view", &autoZoomPan);

			ImVec2 sizeScreenCoord = ImGui::GetContentRegionAvail();
			ImVec2 topLeftWindowCoord = ImGui::GetCursorScreenPos();
			ImVec2 centerWindowCoord = ImVec2(topLeftWindowCoord.x + sizeScreenCoord.x / 2.0f, topLeftWindowCoord.y + sizeScreenCoord.y / 2.0f);
			ImVec2 bottomRightWindowCoord = ImVec2(topLeftWindowCoord.x + sizeScreenCoord.x, topLeftWindowCoord.y + sizeScreenCoord.y);
			ImVec2 canvasMargin = ImVec2(100.0f, 100.0f);
			float canvasDisplaySize = min(sizeScreenCoord.x - canvasMargin.x * 2.0f, sizeScreenCoord.y - canvasMargin.y * 2.0f);

			static float mouseWheelPosition = 0.0f;
			static ImVec2 canvasCenterContentCoord = ImVec2(0.5f, 0.5f);

			static float zoomLevelRatio = 1.0f;
			static bool useZoomTarget = false;
			static float zoomLevelTarget = 1.0f;

			if (autoZoomPan) {
				ImGui::SameLine();
				ImGui::SliderFloat("Zoom multiplier", &zoomOffset, 0.0001f, 100.0f, 0, ImGuiSliderFlags_Logarithmic);
			}
			else {
				ImGui::SameLine();
				if (ImGui::Button("Reset view")) {
					zoomLevelTarget = 1.0f;
					useZoomTarget = true;
					zoomOffset = 0.5f;
					canvasCenterContentCoord = ImVec2(0.5f, 0.5f);
				}

			}

			// catch mouse interactions
			ImGui::InvisibleButton("canvas", sizeScreenCoord, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
			const bool is_hovered = ImGui::IsItemHovered(); // Hovered
			const bool is_active = ImGui::IsItemActive();   // Mouse button held


			/*ImVec2(centerWindowCoord.x - (canvasCenterContentCoord.x - X_COORD) * canvasDisplaySize * zoomLevelRatio,
			centerWindowCoord.y + (canvasCenterContentCoord.y - Y_COORD) * canvasDisplaySize * zoomLevelRatio)*/


			if (ImGui::IsItemHovered()) {

				// zoom level control
				if (!autoZoomPan)
					zoomLevelRatio *= powf(1.05, ImGui::GetIO().MouseWheel);

				zoomLevelRatio = max(zoomLevelRatio, FLT_MIN);

				mouseWheelPosition += ImGui::GetIO().MouseWheel;

				if (!autoZoomPan) {
					canvasCenterContentCoord.x += -(ImGui::GetIO().MousePos.x - centerWindowCoord.x + (centerWindowCoord.x - ImGui::GetIO().MousePos.x) *
						(powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevelRatio;
					canvasCenterContentCoord.y -= -(ImGui::GetIO().MousePos.y - centerWindowCoord.y + (centerWindowCoord.y - ImGui::GetIO().MousePos.y) *
						(powf(1.05, ImGui::GetIO().MouseWheel))) / canvasDisplaySize / zoomLevelRatio;
				}
			}

			if (useZoomTarget) {
				zoomLevelRatio *= powf(zoomLevelTarget / zoomLevelRatio, 0.1);
				if (zoomLevelTarget / zoomLevelRatio < 1.01f && zoomLevelTarget / zoomLevelRatio > 1.0f / 1.01f) {
					useZoomTarget = false;
					zoomLevelRatio = zoomLevelTarget;
				}
			}


			// pan control
			const float mousePanThreshold = contextMenuEnabled ? -1.0f : 0.0f;

			if (!autoZoomPan) {
				if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mousePanThreshold)) {
					canvasCenterContentCoord.x += -io.MouseDelta.x / canvasDisplaySize / zoomLevelRatio;
					canvasCenterContentCoord.y -= -io.MouseDelta.y / canvasDisplaySize / zoomLevelRatio;
				}
			}


			ImGuiIO& io = ImGui::GetIO();
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			drawList->PushClipRect(topLeftWindowCoord, bottomRightWindowCoord, true);

			// Draw border and background color
			drawList->AddRectFilled(topLeftWindowCoord, bottomRightWindowCoord, IM_COL32(0, 0, 0, 255), 6.0f, 0);


			float x_max = -1000000000.0;
			float x_min = 1000000000.0;
			float y_max = -1000000000.0;
			float y_min = 1000000000.0;

			for (int i = 0; i < graph.vertexCount; i++) {
				x_max = max(x_max, graph.vertices[i]->coord.x);
				x_min = min(x_min, graph.vertices[i]->coord.x);
				y_max = max(y_max, graph.vertices[i]->coord.y);
				y_min = min(y_min, graph.vertices[i]->coord.y);
			}

			if (autoZoomPan) {
				canvasCenterContentCoord.x += ((x_max - x_min) / 2.0f + x_min - canvasCenterContentCoord.x) * 0.3f;
				canvasCenterContentCoord.y += ((y_max - y_min) / 2.0f + y_min - canvasCenterContentCoord.y) * 0.3f;

				zoomLevelRatio += (1.0f / max(max(x_max - x_min, y_max - y_min), 0.1f) * zoomOffset - zoomLevelRatio) * 0.3f;
			}



			// Draw 1*1 square
			drawList->AddRectFilled(ImVec2(
				centerWindowCoord.x - (canvasCenterContentCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio,
				centerWindowCoord.y + (canvasCenterContentCoord.y - 1.0f) * canvasDisplaySize * zoomLevelRatio)
				, ImVec2(
					centerWindowCoord.x - (canvasCenterContentCoord.x - 1.0f) * canvasDisplaySize * zoomLevelRatio,
					centerWindowCoord.y + (canvasCenterContentCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio)
				, IM_COL32(40, 40, 40, 255), 0.0f);



			if (showGrid) {

				{
					// vertical lines (secondary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio) - 1);
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerWindowCoord.x - (fmodf(canvasCenterContentCoord.x, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizeScreenCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizeScreenCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing)
						drawList->AddLine(ImVec2(x, centerWindowCoord.y - sizeScreenCoord.y / 2.0f), ImVec2(x, centerWindowCoord.y + sizeScreenCoord.y / 2.0f), IM_COL32(90, 90, 90, 80), 1.0f);
				}
				{
					// vertical lines (primary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio));
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerWindowCoord.x - (fmodf(canvasCenterContentCoord.x, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizeScreenCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizeScreenCoord.x / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					for (float x = startingSnapGridCoord; x <= endingSnapGridCoord; x += screenGridSpacing)
						drawList->AddLine(ImVec2(x, centerWindowCoord.y - sizeScreenCoord.y / 2.0f), ImVec2(x, centerWindowCoord.y + sizeScreenCoord.y / 2.0f), IM_COL32(90, 90, 90, 80), 2.0f);
				}
				{
					// horizontal lines (secondary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio) - 1);
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerWindowCoord.y + (fmodf(canvasCenterContentCoord.y, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizeScreenCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizeScreenCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing)
						drawList->AddLine(ImVec2(centerWindowCoord.x - sizeScreenCoord.x / 2.0f, y), ImVec2(centerWindowCoord.x + sizeScreenCoord.x / 2.0f, y), IM_COL32(90, 90, 90, 80), 1.0f);
				}
				{
					// horizontal lines (primary)
					const float contentGridSpacing = powf(10, -(int)log10f(zoomLevelRatio));
					const float screenGridSpacing = contentGridSpacing * canvasDisplaySize * zoomLevelRatio;
					const float centerSnapGridCoord = centerWindowCoord.y + (fmodf(canvasCenterContentCoord.y, contentGridSpacing) * canvasDisplaySize * zoomLevelRatio);
					const float startingSnapGridCoord = centerSnapGridCoord - (int)(sizeScreenCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;
					const float endingSnapGridCoord = centerSnapGridCoord + (int)(sizeScreenCoord.y / screenGridSpacing / 2.0f + 2.0f) * screenGridSpacing;

					for (float y = startingSnapGridCoord; y <= endingSnapGridCoord; y += screenGridSpacing)
						drawList->AddLine(ImVec2(centerWindowCoord.x - sizeScreenCoord.x / 2.0f, y), ImVec2(centerWindowCoord.x + sizeScreenCoord.x / 2.0f, y), IM_COL32(90, 90, 90, 80), 2.0f);
				}

				ImVec2 contentOriginCenterDelta = ImVec2(centerWindowCoord.x - canvasCenterContentCoord.x * canvasDisplaySize * zoomLevelRatio,
					centerWindowCoord.y + canvasCenterContentCoord.y * canvasDisplaySize * zoomLevelRatio);


				// blue origin dot
				drawList->AddCircleFilled(contentOriginCenterDelta, 5.0f, IM_COL32(0, 211, 255, 255));


				for (int i = 0; i < graph.edgeCount; i++) {
					drawList->AddLine(
						ImVec2(centerWindowCoord.x - (canvasCenterContentCoord.x - graph.edges[i].startingVertex->coord.x) * canvasDisplaySize * zoomLevelRatio,
							centerWindowCoord.y + (canvasCenterContentCoord.y - graph.edges[i].startingVertex->coord.y) * canvasDisplaySize * zoomLevelRatio),
						ImVec2(centerWindowCoord.x - (canvasCenterContentCoord.x - graph.edges[i].endingVertex->coord.x) * canvasDisplaySize * zoomLevelRatio,
							centerWindowCoord.y + (canvasCenterContentCoord.y - graph.edges[i].endingVertex->coord.y) * canvasDisplaySize * zoomLevelRatio),
						IM_COL32(200, 200, 200, 255), 5.0f * powf(zoomLevelRatio, 0.1));
				}

				for (int i = 0; i < graph.vertexCount; i++) {
					drawList->AddCircleFilled(ImVec2(centerWindowCoord.x - (canvasCenterContentCoord.x - graph.vertices[i]->coord.x) * canvasDisplaySize * zoomLevelRatio,
						centerWindowCoord.y + (canvasCenterContentCoord.y - graph.vertices[i]->coord.y) * canvasDisplaySize * zoomLevelRatio), 20.0f * powf(zoomLevelRatio, 0.1), IM_COL32(255, 211, 0, 255));

					ImGui::PushFont(vertexTextFont);
					float font_size = ImGui::GetFontSize() * powf(zoomLevelRatio, 0.1) * std::to_string(i).size() / 2;

					drawList->AddText(vertexTextFont, 36.0f * powf(zoomLevelRatio, 0.1),
						ImVec2(centerWindowCoord.x - (canvasCenterContentCoord.x - graph.vertices[i]->coord.x) * canvasDisplaySize * (zoomLevelRatio)-font_size + (font_size / 2),
							centerWindowCoord.y + (canvasCenterContentCoord.y - graph.vertices[i]->coord.y) * canvasDisplaySize * zoomLevelRatio - 18.0f * powf(zoomLevelRatio, 0.1)),
						IM_COL32(15, 15, 15, 255), std::to_string(i).c_str(), 0, 0.0f, 0);

					ImGui::PopFont();
				}



				{
					// x positive 10x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio + 2.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(0.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// x negative 10x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x + powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio - 52.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// x positive 1x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio + 2.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(0.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// x negative 1x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x + powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio - 52.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - 0.0f) * canvasDisplaySize * zoomLevelRatio);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("X: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// y positive 10x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio - 32.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 1.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				{
					// y negative 10x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y + powf(10, (int)log10f(1.0f / zoomLevelRatio))) * canvasDisplaySize * zoomLevelRatio + 2.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio)));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// y positive 1x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y - powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio - 32.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: 1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 1.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}

				if (fmodf(log10f(1.0f / zoomLevelRatio), 1.0f) <= 0.5f) {
					// y negative 1x
					ImVec2 gridCoordLabelMinPos(centerWindowCoord.x - (canvasCenterContentCoord.x - 0.0f) * canvasDisplaySize * zoomLevelRatio - 50.0f,
						centerWindowCoord.y + (canvasCenterContentCoord.y + powf(10, (int)log10f(1.0f / zoomLevelRatio) - 1)) * canvasDisplaySize * zoomLevelRatio + 2.0f);
					ImVec2 gridCoordLabelMaxPos(gridCoordLabelMinPos.x + 50.0f, gridCoordLabelMinPos.y + 30.0f);
					std::string tempStr("Y: -1E" + std::to_string((int)log10f(1.0f / zoomLevelRatio) - 1));
					char* label = new char[tempStr.length() + 1];
					std::strcpy(label, tempStr.c_str());
					ImVec2 labelSize = ImGui::CalcTextSize(label, NULL, true);
					ImVec2 labelAlign(1.0f, 0.0f);
					const ImRect bb(gridCoordLabelMinPos, gridCoordLabelMaxPos);

					ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 200));
					ImGui::RenderTextClipped(gridCoordLabelMinPos, gridCoordLabelMaxPos, label, 0, &labelSize, labelAlign, &bb);
					ImGui::PopStyleColor(1);
				}


			}

			if (!autoZoomPan) 
				drawList->AddText(ImVec2(topLeftWindowCoord.x + 4.0f, bottomRightWindowCoord.y - 20.0f), IM_COL32(200, 200, 200, 200), "Pan: Hold right mouse button   Zoom: Scroll", 0);
			else 
				drawList->AddText(ImVec2(topLeftWindowCoord.x + 4.0f, bottomRightWindowCoord.y - 20.0f), IM_COL32(200, 200, 200, 200), "Auto adjust view enabled, manual pan and zoom is disabled", 0);


			drawList->PopClipRect();

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
#pragma once

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

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_MAXIMIZED, true);

        glfwWindow = glfwCreateWindow(config::width, config::height, "Graphene", NULL, NULL);

        if (glfwWindow == NULL)
            return false;

        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cerr << "Failed to initialize OpenGL loader!\n";
            return false;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Window Docking
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        ImGui::GetIO().ConfigDockingAlwaysTabBar = true;
        ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        ImGui::GetIO().IniFilename = "Graphene.ini";
        ImGui::GetIO().IniSavingRate = 1;
        ImGui::GetIO().Fonts->AddFontFromFileTTF(".\\Barlow-Regular.ttf", 16.0f);

        /// TEMPORARY THEME TODO
        ImGui::GetStyle().FrameRounding = 6.0f;
        ImGui::GetStyle().WindowPadding = ImVec2(16.0f, 16.0f);
        ImGui::GetStyle().FramePadding = ImVec2(4.0f, 4.0f);
        ImGui::GetStyle().GrabMinSize = 16.0f;
        ImGui::GetStyle().WindowRounding = 6.0f;
        ImGui::GetStyle().ChildRounding = 6.0f;
        ImGui::GetStyle().PopupRounding = 6.0f;
        ImGui::GetStyle().TabRounding = 6.0f;
        ImGui::GetStyle().GrabRounding = 6.0f;
        ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_None;

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
        colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.03f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.90f, 0.90f, 0.53f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 0.45f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.64f, 0.64f, 0.64f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.93f, 0.93f, 0.93f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.80f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.31f, 0.57f, 0.76f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.69f, 0.81f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.32f, 0.54f, 0.70f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.64f, 0.81f, 0.92f, 1.00f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.55f, 0.68f, 0.76f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.33f, 0.60f, 0.79f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.71f, 0.82f, 0.91f, 1.00f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.59f, 0.75f, 0.86f, 1.00f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.46f, 0.65f, 0.78f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 0.62f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.11f, 0.47f, 0.73f, 1.00f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.04f, 0.31f, 0.49f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.35f, 0.35f, 0.35f, 0.17f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25f, 0.60f, 0.84f, 1.00f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.12f, 0.46f, 0.69f, 1.00f);
        colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.61f, 0.77f, 0.88f, 1.00f);
        colors[ImGuiCol_TabActive] = ImVec4(0.46f, 0.69f, 0.85f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.88f, 0.88f, 0.88f, 0.99f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.42f, 0.72f, 0.41f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.45f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.78f, 0.87f, 0.98f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.57f, 0.57f, 0.64f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.68f, 0.68f, 0.74f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.30f, 0.30f, 0.30f, 0.09f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.69f, 1.00f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.37f, 0.75f, 1.00f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.36f, 0.74f, 1.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.70f, 0.70f, 0.70f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

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
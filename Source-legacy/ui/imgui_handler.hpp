#pragma once

#include "imgui_include.hpp"

namespace ui {
    GLFWwindow* window;

    void glfwErrorCallback(int error, const char* description) {
        ERR("glfw error: [" + std::to_string(error) + "] " + std::string(description));
    }

    int init() {
        glfwSetErrorCallback(glfwErrorCallback);
        if (!glfwInit())
            return -1;

        glfwWindowHint(GLFW_MAXIMIZED, false);
        glfwWindowHint(GLFW_SAMPLES, 4);

        window = glfwCreateWindow(1280, 720, u8"Graphene α", NULL, NULL);
        if (window == NULL)
            return 2;

        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
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
        //ImGui::GetIO().MouseDrawCursor = true;

        ImGui::Spectrum::StyleColorsSpectrum();
        setTheme();

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
        ImGui::GetIO().Fonts->AddFontFromFileTTF("./Barlow-Medium.ttf", 16.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesDefault());

        config.MergeMode = true;
        // fallback font for Chinese characters (default: Microsoft JhengHei UI Regular)
        ImGui::GetIO().Fonts->AddFontFromFileTTF("./msjh.ttf", 16.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

        // mono font for graph display
        config.MergeMode = false;
        vertexTextFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("./Barlow-Regular.ttf", 54.0f, &config, ImGui::GetIO().Fonts->GetGlyphRangesDefault());
        // and framerate display/graph update rate display
        framerateTextFont = ImGui::GetIO().Fonts->AddFontFromFileTTF("./Barlow-Medium.ttf", 30.0f, &config, ImGui::GetIO()
                .Fonts->GetGlyphRangesDefault());

        ImGui::CreateContext();

        return 0;
    }
}
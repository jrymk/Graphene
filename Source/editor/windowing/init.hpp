/// handles glfw and imgui initialization

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace gph {
    namespace editor {
        GLFWwindow *window;

        void glfwErrorCallback(int error, const char *description) {
            std::cerr << "glfw error [" << error << "]: " << description << "\n";
        }

        bool init() {
            glfwSetErrorCallback(glfwErrorCallback);
            if (!glfwInit()) {
                std::cerr << "glfw error: init failed\n";
                return false;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_MAXIMIZED, false);

            window = glfwCreateWindow(1280, 720, u8"Graphene 1.1", nullptr, NULL);

            if (window == nullptr) {
                std::cerr << "glfw error: window creation failed\n";
                return false;
            }

            glfwMakeContextCurrent(window);

            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                std::cerr << "glad error: load failed\n";
                return false;
            }

            // disable vsync
            glfwSwapInterval(0);

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330");

            // preparing for the "next" frame
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            return false;
        }

    }
}
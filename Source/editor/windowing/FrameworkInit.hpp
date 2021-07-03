/// Initialize or recreate and terminate windows matching the given configurations

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace graphene {
    namespace editor {
        GLFWwindow* window = nullptr;

        struct frameworkConfig {
            std::string title = u8"Graphene 1.1";
            bool maximized = false;
            int windowWidth = 1280;
            int windowHeight = 720;
            int antiAliasingSamples = 4;
            bool verticalSync = true;
        };

        void glfwErrorCallback(int error, const char* description) {
            std::cerr << "glfw error [" << error << "]: " << description << "\n";
        }

        bool frameworkTerminate() {
            // cleans up everything after closing the window
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            glfwDestroyWindow(window);
            glfwTerminate();
            window = nullptr;
            return true;
        }

        ///@brief Initializes framework and window, call again to update window settings after calling frameworkTerminate()
        bool frameworkInit(const frameworkConfig& config) {
            // check if previous window is terminated correctly
            if (window != nullptr) {
                std::cerr << "framework warning: call frameworkTerminate() before calling frameworkInit() again\n";
                frameworkTerminate();
            }

            glfwSetErrorCallback(glfwErrorCallback);
            if (!glfwInit()) {
                std::cerr << "glfw error: init failed\n";
                return false;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_SAMPLES, config.antiAliasingSamples);
            glfwWindowHint(GLFW_MAXIMIZED, config.maximized);

            window = glfwCreateWindow(config.windowWidth, config.windowHeight, config.title.c_str(), nullptr, nullptr);

            if (window == nullptr) {
                std::cerr << "glfw error: window creation failed\n";
                return false;
            }

            glfwMakeContextCurrent(window);

            glfwSwapInterval(config.verticalSync ? 1 : 0); // disable vsync

            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
                std::cerr << "glad error: failed to initialize OpenGL context\n";
                return false;
            }

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330");

            return true;
        }
    }
}
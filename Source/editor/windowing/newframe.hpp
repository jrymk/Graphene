/// handles things related to glfw and imgui that happen every frame

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "init.hpp"

namespace gph {
    namespace editor {
        bool newFrame() {
            if (glfwWindowShouldClose(window)) {
                // cleans up everything after closing the window
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();

                ImGui::DestroyContext();

                glfwDestroyWindow(window);
                glfwTerminate();

                return false;
            }

            // renders the window
            ImGui::Render();

            int displayW, displayH;
            glfwGetFramebufferSize(window, &displayW, &displayH);

            glViewport(0, 0, displayW, displayH);

            glClearColor(80, 80, 80, 255);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);

            // prepare for the "next" frame
            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        }
    }
}
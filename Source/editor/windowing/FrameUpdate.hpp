/// perform render and draw calls every frame

#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <glad.h>
#include <GLFW/glfw3.h>

#include "FrameworkInit.hpp"

namespace graphene {
    namespace editor {
        bool newFrame() {
            if (glfwWindowShouldClose(window)) {
                frameworkTerminate();
                return false;
            }

            glfwPollEvents();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

            return true;
        }

        void endFrame() {
            ImGui::EndFrame();

            // renders the window
            ImGui::Render();

            // update viewport size and clear the screen
            int displayW, displayH;
            glfwGetFramebufferSize(window, &displayW, &displayH);
            glViewport(0, 0, displayW, displayH);
            glClearColor(80, 80, 80, 255);
            glClear(GL_COLOR_BUFFER_BIT);

            // draws the content and swap buffers
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            glfwSwapBuffers(window);
        }
    }
}
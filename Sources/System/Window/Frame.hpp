#pragma once

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iosfwd>
#include <queue>
#include <minitrace.h>

namespace gfn::window {
    extern GLFWwindow* glfwWindow;
    std::queue<std::string> dockBuildWindow;

    void preFrame() {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGuiWindowFlags dockspaceWindowFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoDocking;

        ImGui::Begin("ViewportDockspace", nullptr, dockspaceWindowFlags);
        ImGuiID viewportDockspaceId = ImGui::GetID("ViewportDockspace");
        ImGui::DockSpace(viewportDockspaceId, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_PassthruCentralNode);

        ImGuiWindowFlags documentWindowFlags = ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Graph View", nullptr, documentWindowFlags);
        ImGui::PopStyleVar(2);
        ImGuiID documentDockspaceId = ImGui::GetID("DocumentDockspace");
        ImGui::DockSpace(documentDockspaceId, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_PassthruCentralNode);
        // default dock for new file
        while (!dockBuildWindow.empty()) {
            if (ImGui::FindWindowByName(dockBuildWindow.front().c_str())) {
                ImGui::DockBuilderDockWindow(dockBuildWindow.front().c_str(), documentDockspaceId);
                dockBuildWindow.pop();
            } else
                break;
        }

        ImGui::End();
    }

    void postFrame() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfwWindow);
    }

} // namespace gfn::window
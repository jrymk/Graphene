#include "Graphics.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Core/Core/Meta.h>

namespace gfn {
    bool Graphics::launchWindow(Preferences* prefs) {
        if (!glfwInit())
            return false;

        const char* glslVersion = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3 supports 32 bit indices
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        glfwWindowHint(GLFW_SAMPLES, prefs->graphics_antialiasing_samples);
        glfwWindowHint(GLFW_MAXIMIZED, prefs->graphics_launch_maximized);

        glfwWindow = glfwCreateWindow(prefs->graphics_launch_window_width,
                                      prefs->graphics_launch_window_height,
                                      buildVersion, nullptr, nullptr);
        if (!glfwWindow)
            return false;

        glfwMakeContextCurrent(glfwWindow);
        glfwSwapInterval(prefs->graphics_vertical_sync ? 1 : 0);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            return false;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Window Docking
        ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
        ImGui::GetIO().BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

        ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
        ImGui_ImplOpenGL3_Init(glslVersion);

        ImGui::GetIO().IniFilename = "Graphene.ini";
        ImGui::GetIO().IniSavingRate = 1;
        ImGui::GetIO().Fonts->AddFontFromFileTTF(prefs->graphics_default_font.c_str(), prefs->graphics_font_size);

        return true;
    }

    void Graphics::closeWindow() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(glfwWindow);
        glfwTerminate();
    }

    void Graphics::preFrame() {
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

        ImGuiWindowFlags documentWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
        ImGui::Begin("Graph View", nullptr, documentWindowFlags);
        ImGui::PopStyleVar(2);
        ImGuiID documentDockspaceId = ImGui::GetID("DocumentDockspace");
        ImGui::DockSpace(documentDockspaceId, ImVec2(0.0f, 0.0f),
                         ImGuiDockNodeFlags_PassthruCentralNode);
        // default dock for new file
        /*while (!dockBuildWindow.empty()) {
            if (ImGui::FindWindowByName(dockBuildWindow.front().c_str())) {
                ImGui::DockBuilderDockWindow(dockBuildWindow.front().c_str(), documentDockspaceId);
                dockBuildWindow.pop();
            } else
                break;
        }*/

        ImGui::End();
    }

    void Graphics::postFrame() {
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(glfwWindow);
    }

    bool Graphics::shouldClose() {
        return glfwWindowShouldClose(glfwWindow);
    }
}
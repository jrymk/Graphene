#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <queue>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Editor/Preferences/Preferences.h>

namespace gfn {
    class Graphics {
    public:
        GLFWwindow* glfwWindow;
        std::queue<std::string> dockBuildWindow;
        ImFont* fontSmall;
        ImFont* fontScalable;

        bool launchWindow(Preferences* prefs);

        void closeWindow();

        void preFrame();

        void postFrame();

        bool shouldClose();
    };
}

#endif
#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "../ExceptionHandler.hpp"
#include "Struct.hpp"

class Window {
private:
	GLFWwindow* m_window = nullptr;
	Vec2u m_windowSize;
	
public:
	
	void createWindow(Vec2u windowSize, const std::string& title) {

		m_windowSize = windowSize;

		// initialize glfw library
		glfwInit();

		// setup window properties
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_FOCUSED, true);
		glfwWindowHint(GLFW_RESIZABLE, true);

		// create the window
		m_window = glfwCreateWindow(windowSize.x, windowSize.y, title.c_str(), nullptr, nullptr);

		if (m_window == nullptr) {
			ExceptionHandler eh;
			// window creation failed
			eh.err("OpenGL window creation failed");
			return;
		}

		// set OpenGL context to the window
		glfwMakeContextCurrent(m_window);

		// setup viewport
		glViewport(0, 0, windowSize.x, windowSize.y);

		// disable Vsync
		glfwSwapInterval(0);
		
	}

	void closeWindow() {
		// close window and clear glfw
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	
};

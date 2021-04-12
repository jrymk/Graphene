﻿#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "InitGlfw.hpp"
#include "InitGlew.hpp"
#include "../../ExceptionHandler.hpp"
#include "Structures.hpp"

namespace gue {
	class Window {
	private:
		GLFWwindow* m_window = nullptr;
		Vec2u m_windowSize;

	public:
		GLFWwindow* getGLFWWindow() {
			return m_window;
		}

		static void draw(GLFWwindow* window) {
			// Rendering code goes here
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
		}

		static void windowSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			
			//draw(window);
		}

		static void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
			glViewport(0, 0, width, height);
			//draw(window);
		}
		
		bool createWindow(Vec2u windowSize, const char* title) {
			gue::initGlfw();
			
			DBG("creating window with size " + std::to_string(windowSize.x) + ", " + std::to_string(windowSize.y));
			m_windowSize = windowSize;

			// setup window properties
			glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // using OpenGL 3.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_FOCUSED, true);
			glfwWindowHint(GLFW_RESIZABLE, true);

			// create the window
			m_window = glfwCreateWindow(windowSize.x, windowSize.y, title, nullptr, nullptr);

			if (m_window == nullptr) {
				// window creation failed
				ERR("OpenGL window creation failed");
				glfwTerminate();
				return false;
			}

			// set OpenGL context to the window
			glfwMakeContextCurrent(m_window);

			// initialize glew after creating OpenGL context
			gue::initGlew();
			
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // use core profile

			// setup viewport
			glViewport(0, 0, windowSize.x, windowSize.y);

			// updates viewport after screen resize
			//glfwSetWindowSizeCallback(m_window, windowSizeCallback);
			glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
			
			// disable Vsync
			glfwSwapInterval(0);

			OK("window created successfully");
			return true;
		}

		void closeWindow() {
			// close window and clear glfw
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		Vec2u getFramebufferSize() {
			int width, height;
			glfwGetFramebufferSize(m_window, &width, &height);
			return Vec2i( width, height ).toUnsigned();
		}

	};
}
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../ExceptionHandler.hpp"

namespace gue {
	
	bool initGlfw() {
		if (!glfwInit()) {
			ERR("Failed to initialize GLFW\n");
			return false;
		}
		return true;
	}

	
	bool initGlad() {
		// initialize glad after creating OpenGL context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			ERR("Failed to initialize GLAD\n");
			return false;
		}
		return true;
	}
	
	
}
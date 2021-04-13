#pragma once
#include <GLFW/glfw3.h>
#include "../../ExceptionHandler.hpp"

namespace gue {
	bool initGlfw() {
		if (!glfwInit()) {
			ERR("Failed to initialize GLFW\n");
			return false;
		}

	}
	
}
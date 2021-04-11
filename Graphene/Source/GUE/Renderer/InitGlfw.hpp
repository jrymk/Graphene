#pragma once
#include <GLFW/glfw3.h>
#include "../../ExceptionHandler.hpp"

namespace gue {
	bool initGlfw() {
		if (!glfwInit()) {
			ExceptionHandler::err("Failed to initialize GLFW\n");
			return false;
		}

	}
	
}
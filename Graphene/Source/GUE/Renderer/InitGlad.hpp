#pragma once
#include <glad/glad.h>
#include "../../ExceptionHandler.hpp"

namespace gue {
	bool initGlad() {
		// initialize glad after creating OpenGL context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			ERR("Failed to initialize GLAD\n");
			return false;
		}
	}
}
	
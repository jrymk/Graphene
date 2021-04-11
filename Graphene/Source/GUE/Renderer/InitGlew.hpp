#pragma once
#include <gl/glew.h>
#include "../../ExceptionHandler.hpp"

namespace gue {
	bool initGlew() {
		// initialize glew after creating OpenGL content
		glewExperimental = true; // Needed for core profile

		if (glewInit() != GLEW_OK) {
			ERR("Failed to initialize GLEW\n");
			return false;
		}
	}
}
	
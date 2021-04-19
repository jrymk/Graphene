#pragma once
#include "ExceptionHandler.hpp"

static void glfw_error_callback(int error, const char* description) {
	ERR("GLFW Error: " + std::to_string(error) + std::string(description));
}


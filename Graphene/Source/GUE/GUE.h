#pragma once

#define GLFW_INCLUDE_NONE

#include "Batch/BatchInclude.h"

#include "Elements/Element.hpp"
#include "Elements/RectangleShape.hpp"
#include "Elements/CircleShape.hpp"
#include "Elements/RoundedRectangleShape.hpp"
#include "Elements/LineShape.hpp"
#include "Elements/PaddingLayout.hpp"
#include "Renderer/Window.hpp"
#include "Renderer/Shaders.hpp"
#include "Renderer/ProfilerUtils.hpp"

#include "System/Timer.hpp"
#include "Defines/Colors.hpp"
#include "../ExceptionHandler.hpp"

// Use dedicated graphic cards by default
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Graphene User Interface Engine
// 
// High performance adaptive graphical user interface engine using OpenGL designed for the Graphene project
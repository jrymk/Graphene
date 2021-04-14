#pragma once

#define GLFW_INCLUDE_NONE
#include "Elements/Element.hpp"
#include "Elements/RectangleElement.hpp"
#include "Elements/CircleElement.hpp"
#include "Elements/RoundedRectangle.hpp"
#include "Elements/LineElement.hpp"
#include "Elements/PaddingElement.hpp"
#include "Renderer/Window.hpp"
#include "Renderer/Shaders.hpp"
#include "Renderer/Batch.hpp"
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
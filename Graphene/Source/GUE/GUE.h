#pragma once

#include "Renderer/Window.hpp"
#include "Elements/Element.hpp"
#include "Elements/CircleElement.hpp"
#include "Elements/RectangleElement.hpp"
#include "Elements/PaddingElement.hpp"
#include "Renderer/Shaders.hpp"
#include "System/Timer.hpp"
#include "Renderer/ProfilerUtils.hpp"

// Use dedicated graphic cards by default
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Graphene User Interface Engine
// 
// High performance adaptive graphical user interface engine using OpenGL designed for the Graphene project
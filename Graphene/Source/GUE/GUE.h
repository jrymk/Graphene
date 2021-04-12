#pragma once

#include "Renderer/Window.hpp"
#include "Elements/Element.hpp"
#include "Elements/CircleElement.hpp"
#include "Renderer/Shaders.hpp"

// Use dedicated graphic cards by default
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// Graphene User Interface Engine
// 
// High performance adaptive graphical user interface engine using OpenGL designed for the Graphene project
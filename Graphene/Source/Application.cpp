#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "GUE/GUE.h"
#include "ExceptionHandler.hpp"


int main() {
	gue::Window window;
	if (!window.createWindow(Vec2u(1024, 768), u8"Graphene α")) {
		ERR("failed to create window");
	}

	gue::VertexArray vertexArray(window);

	gue::Shader shader;
	shader.setShaderSource(shader.defaultVertexShader, shader.defaultFragmentShader);
	shader.loadShader();


	glfwSetInputMode(window.getGLFWWindow(), GLFW_STICKY_KEYS, GL_TRUE);

	ExceptionHandler eh;
	eh.flushExceptionsToIOStream();

	gue::Timer animTimer;

	gue::FramerateCounter framerateCounter;

	do {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		vertexArray.clear();
		
		auto roundedRect = new gue::RoundedRectangle("rounded rect", { 0.1, 0 }, { 0.1, 0 }, { 0.8, 0 }, { 0.8, 0 }, { 0, 30 }, Color(200, 200, 200, 255));

		auto circle = new gue::CircleElement("circle", { 0.5f, 0.0f }, { 0.5f, 0.0f }, { 0.5f, 0.0f }, Color(255, 211, 0, 255));
		roundedRect->add(circle, -1);
		
		roundedRect->build(&vertexArray, {0, 0}, window.getFramebufferSize().toFloat());


		vertexArray.draw(shader);

		//framerate counter
		framerateCounter.frameCount();
		DBG(std::to_string(framerateCounter.getFramerate()) + "fps\tTriangles: " + std::to_string(vertexArray.getVertices()->size() / 3));

		//DBG(std::to_string(m_vertexArray.getVertices()->size()));

		// Swap buffers
		GLCall(glfwSwapBuffers(window.getGLFWWindow()));
		GLCall(glfwPollEvents());

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window.getGLFWWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.getGLFWWindow()) == 0);

}
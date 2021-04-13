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
		
		//vertexArray.printContents();


		auto circle = new gue::CircleElement("root circle");
		circle->x = { 0.5f, 0.0f };
		circle->y = { 0.5f, 0.0f };
		circle->radius = { 0.5f, 0.0f };
		circle->color = { 255, 255, 255, 255 };
		//vertexArray.printContents();

		for (int i = 0; i < 60; i++) {
			for (int j = 0; j < 60; j++) {
				auto circle2 = new gue::CircleElement(std::to_string(i) + ", " + std::to_string(j));
				circle2->x = { (float)i / 60.0f, 20 * (float)cos((animTimer.getSeconds() + i + j) * 0.5) };
				circle2->y = { (float)j / 60.0f, 20 * (float)sin((animTimer.getSeconds() + i + j) * 0.4) };
				circle2->radius = { 0.01f, 0.0f };
				circle2->color = { (uint8_t)(i * 255 / 60), (uint8_t)(j * 255 / 60), (uint8_t)(round((float)127 * cos((animTimer.getSeconds() + (float)i * 0.2 - (float)j * 0.3) * 0.5) + 127)), 150 };
				//circle->color = { 255, 211, 0, 5 };

				circle->add(circle2, -1);
				//vertexArray.printContents();
			}
		}
		
		circle->build(&vertexArray, { 0, 0 }, { window.getFramebufferSize().toFloat().x, window.getFramebufferSize().toFloat().y });
		circle->deleteElement();
		
		vertexArray.draw(shader);
		
		//framerate counter
		framerateCounter.frameCount();
		DBG(std::to_string(framerateCounter.getFramerate()) + "fps\tTriangles: " + std::to_string(vertexArray.getVertices()->size() / 3));
		
		//DBG(std::to_string(vertexArray.getVertices()->size()));

		// Swap buffers
		GLCall(glfwSwapBuffers(window.getGLFWWindow()));
		GLCall(glfwPollEvents());

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window.getGLFWWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.getGLFWWindow()) == 0);

}
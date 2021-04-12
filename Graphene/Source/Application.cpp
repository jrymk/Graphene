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

	double lastTime = glfwGetTime();
	int nbFrames = 0;

	ExceptionHandler eh;
	eh.flushExceptionsToIOStream();


	gue::FramerateCounter framerateCounter;

	do {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		vertexArray.clear();

		//vertexArray.allocate(800000, 2000000);

		auto circle = new gue::CircleElement;
		circle->x = { 0.5f, 0.0f };
		circle->y = { 0.5f, 0.0f };
		circle->radius = { 0.5f, 0.0f };
		circle->color = { 255, 255, 255, 255 };
		circle->generateBuffer(&vertexArray, { 0, 0 }, { window.getFramebufferSize().toFloat().x, window.getFramebufferSize().toFloat().y });
		//vertexArray.printContents();
		delete circle;

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				auto circle2 = new gue::CircleElement;
				circle2->x = { (float)i / 20.0f, 0.0f };
				circle2->y = { (float)j / 20.0f, 0.0f };
				circle2->radius = { 0.01f, 0.0f };
				circle2->color = { (uint8_t)i, (uint8_t)j, 0, 150 };
				//circle->color = { 255, 211, 0, 5 };

				circle2->generateBuffer(&vertexArray, { 0, 0 }, { window.getFramebufferSize().toFloat().x, window.getFramebufferSize().toFloat().y });
				//vertexArray.printContents();
				delete circle2;
			}
		}
		//vertexArray.printContents();

		vertexArray.draw(shader);
		
		//framerate counter
		framerateCounter.frameCount();
		DBG(std::to_string(framerateCounter.getFramerate()) + "fps");
		
		//DBG(std::to_string(vertexArray.getVertices()->size()));

		// Swap buffers
		GLCall(glfwSwapBuffers(window.getGLFWWindow()));
		GLCall(glfwPollEvents());

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window.getGLFWWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window.getGLFWWindow()) == 0);

}
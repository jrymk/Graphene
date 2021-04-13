﻿#include <cstdio>
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
		circle->y = { 0.f, 0.0f };
		circle->radius = { 0.5f, 0.0f };
		circle->fillColor = { 255, 0, 210, 100 };
		//circle->backgroundColor = { 255, 211, 0, 255 };



		auto padding = new gue::PaddingElement("padding", { 0.1f, 0.0f }, { 0.0f, 100.0f }, { 0.2f, 0.0f }, { 0.0f, 200.0f });
		//padding->backgroundColor = Color(100, 100, 100, 100);
		//padding->fillColor = Color(110, 255, 255, 200);


		circle->add(padding, -1);

		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 20; j++) {
				auto circle2 = new gue::CircleElement(std::to_string(i) + ", " + std::to_string(j));
				circle2->x = { (float)i / 20.0f, 30 * (float)cos((animTimer.getSeconds() * 3.0f + i + j) * 0.5) };
				circle2->y = { (float)j / 20.0f, 30 * (float)sin((animTimer.getSeconds() * 3.0f + i + j) * 0.4) };
				circle2->radius = { 0.1f, 0.0f };
				circle2->fillColor = { (uint8_t)(i * 255 / 20), (uint8_t)(j * 255 / 20), (uint8_t)(round((float)127 * cos((animTimer.getSeconds() * 3.0f + (float)i * 0.2 - (float)j * 0.3) * 0.5) + 127)), 60 };
				//circle->color = { 255, 211, 0, 5 };

				circle->add(circle2, -1);
				//vertexArray.printContents();
			}
		}

		/*auto rectr = new gue::RectangleElement("rectangle", { 0.3f, 0.0f }, { 0.5f, 0.0f }, { 0.6f, 0.0f }, { 1.0f, 0.0f }, Color(211, 211, 0, 255));
		rectr->backgroundColor = Color(100, 255, 100, 200);
		padding->add(rectr, -1);*/

		auto line = new gue::LineElement("line", { 0.1f, 0.0f }, { 0.2f, 0.0f }, { 1.0f, -200.0f }, { 0.5f, 0.0f }, { 0.0f, 50.0f }, Color(255, 255, 255, 255));
		circle->add(line, -1);

		auto line2 = new gue::LineElement("line", { 0.1f, 0.0f }, { 0.2f, 0.0f }, { 1.0f, -200.0f }, { 0.6f, 0.0f }, { 0.0f, 10.0f }, Color(255, 255, 255, 255));
		circle->add(line2, -1);

		auto line3 = new gue::LineElement("line", { 0.1f, 0.0f }, { 0.2f, 0.0f }, { 1.0f, -200.0f }, { 0.7f, 0.0f }, { 0.0f, 3.0f }, Color(255, 255, 255, 255));
		circle->add(line3, -1);

		auto line4 = new gue::LineElement("line", { 0.1f, 0.0f }, { 0.2f, 0.0f }, { 1.0f, -200.0f }, { 0.8f, 0.0f }, { 0.0f, 1.0f }, Color(255, 255, 255, 255));
		circle->add(line4, -1);





		circle->build({ 0, 0 }, { window.getFramebufferSize().toFloat().x, window.getFramebufferSize().toFloat().y });
		circle->push(&vertexArray);

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
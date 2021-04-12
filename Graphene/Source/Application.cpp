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

	ExceptionHandler eh;
	eh.flushExceptionsToIOStream();


	const std::string vertexShader = "#version 330 core\n\
									  layout(location = 0) in vec2 aPosition;\n\
									  layout(location = 1) in vec4 aColor;\n\
									  out vec4 vertexColor;\n\
									  \n\
									  void main() {\n\
										  vertexColor = vec4(aColor.rgba);\n\
										  gl_Position.xy = aPosition;\n\
										  gl_Position.z = 1.0;\n\
										  gl_Position.w = 1.0;\n\
									  }\n";

	const std::string fragmentShader = "#version 330 core\n\
									   out vec4 FragColor;\n\
									   in vec4 vertexColor;\n\
									   \n\
									   void main() {\n\
										   FragColor = vertexColor;\n\
									   \n}";


	gue::Shader shader;
	shader.setShaderSource(vertexShader, fragmentShader);
	shader.loadShader();


	glfwSetInputMode(window.getGLFWWindow(), GLFW_STICKY_KEYS, GL_TRUE);


	do {
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		vertexArray.clear();

		auto circle = new gue::CircleElement;
		circle->x = { 0.5f, 0.0f };
		circle->y = { 0.5f, 0.0f };
		circle->radius = { 0.5f, 0.0f };
		circle->color = { 255, 255, 255, 255 };
		circle->generateBuffer(&vertexArray, { 0, 0 }, { window.getFramebufferSize().toFloat().x, window.getFramebufferSize().toFloat().y });
		//vertexArray.printContents();
		delete circle;
		
		for (int i = 0; i < 100; i++) {
			for (int j = 0; j < 100; j++) {
				auto circle2 = new gue::CircleElement;
				circle2->x = { (float)i / 100.f, 0.0f };
				circle2->y = { (float)j / 100.0f, 0.0f };
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

		DBG("Framerate: " + std::to_string((int)round((double)1.0 / glfwGetTime())) + "\tTriangles: " + std::to_string(vertexArray.getVertices()->size()));
		glfwSetTime(0);
		
		//DBG(std::to_string(vertexArray.getVertices()->size()));
		
		// Swap buffers
		GLCall(glfwSwapBuffers(window.getGLFWWindow()));
		GLCall(glfwPollEvents());

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window.getGLFWWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window.getGLFWWindow()) == 0);

}
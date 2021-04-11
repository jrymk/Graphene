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

	gue::VertexArray vertexArray;

	gue::CircleElement circle;
	circle.x = { 0.0f, 0.0f };
	circle.y = { 0.0f, 0.0f };
	circle.radius = { 0.8f, 0.8f };
	circle.color = { 1.0f, 0.82745f, 0.0f, 1.0f };

	vertexArray.clear();
	circle.generateBuffer(&vertexArray, { 0, 0 }, { 1.0, 1.0 });

	vertexArray.printContents();

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

		
		vertexArray.draw(shader);

		// Swap buffers
		GLCall(glfwSwapBuffers(window.getGLFWWindow()));
		GLCall(glfwPollEvents());

		ExceptionHandler eh;
		eh.flushExceptionsToIOStream();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window.getGLFWWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window.getGLFWWindow()) == 0);

}
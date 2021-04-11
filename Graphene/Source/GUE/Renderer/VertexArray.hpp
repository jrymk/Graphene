#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>

namespace gue {
	class vertexArray {
		GLuint m_vertexBufferObject;

		
		void loadContent() {
			glGenBuffers(1, &m_vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
						
		}
		
		
	};
}
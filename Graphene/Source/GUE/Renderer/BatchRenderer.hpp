#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "Batch.hpp"

namespace gue {
	class BatchRenderer {

	private:


		GLuint m_vertexBufferObject;

		std::vector<float> vertexBuffer;


		void loadContent() {
			glGenBuffers(1, &m_vertexBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);

		}
	};
	
}